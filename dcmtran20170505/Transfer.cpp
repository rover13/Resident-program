#include "Transfer.h"
#include <QtNetwork>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmnet/diutil.h>


struct Transfer::Imp
{
	bool is_ok;
	T_ASC_Network* net;
	T_ASC_Parameters* params;
	T_ASC_Association* assoc;

	~Imp();
	Imp(QString host, QString title, int port);
	QString store(QString fname);
	OFCondition add_contexts();
};


Transfer::Transfer(QString host, QString title, int port) : imp(new Imp(host, title, port)) {}


Transfer::~Transfer() {}


bool
Transfer::isValid() const
{
	return imp->is_ok;
}


QString
Transfer::store(QString fname)
{
	return imp->store(fname);
}



#define countof(x) (sizeof(x) / sizeof(x[0]))

extern GlobalDcmDataDictionary dcmDataDict;

static auto const opt_timeout = 30;
static auto const opt_maxReceivePDULength = ASC_DEFAULTMAXPDU;
static auto const opt_blockMode = DIMSE_BLOCKING;
static auto const opt_dimseTimeout = 0;
static auto const opt_ourTitle = "STORESCU";


Transfer::Imp::Imp(QString host, QString title, int port)
	: is_ok(false), net(nullptr), params(nullptr), assoc(nullptr)
{
	OFCondition cond;

	if (!dcmDataDict.isDictionaryLoaded()) return;

	cond = ASC_initializeNetwork(NET_REQUESTOR, 0, opt_timeout, &net);
	if (cond.bad()) return;

	cond = ASC_createAssociationParameters(&params, opt_maxReceivePDULength);
	if (cond.bad()) return;

	cond = ASC_setAPTitles(params, opt_ourTitle, title.toUtf8().constData(), NULL);
	if (cond.bad()) return;

	cond = ASC_setTransportLayerType(params, false);
	if (cond.bad()) return;

	auto localHost = QHostInfo::localHostName();
	auto peerHost = QString("%1:%2").arg(host).arg(port);
	cond = ASC_setPresentationAddresses(params, localHost.toUtf8().constData(),
										peerHost.toUtf8().constData());
	if (cond.bad()) return;

	cond = add_contexts();
	if (cond.bad()) return;

	cond = ASC_requestAssociation(net, params, &assoc);
	if (cond.bad()) return;

	if (ASC_countAcceptedPresentationContexts(params) == 0) return;

	is_ok = true;
}


Transfer::Imp::~Imp()
{
	if (assoc) {
		ASC_releaseAssociation(assoc);
		ASC_destroyAssociation(&assoc);
	}

	if (net) {
		ASC_dropNetwork(&net);
	}
}


QString
Transfer::Imp::store(QString fname)
{
	if (!is_ok) return "";
	OFCondition cond;

	DcmFileFormat dcmff;
	cond = dcmff.loadFile(fname.toStdWString().c_str());
	if (cond.bad()) return cond.text();

	DIC_UI sopClass;
	DIC_UI sopInstance;
	if (!DU_findSOPClassAndInstanceInDataSet(dcmff.getDataset(), sopClass, sopInstance)) {
		cond = DIMSE_BADDATA;
		return cond.text();
	}

	DcmXfer filexfer(dcmff.getDataset()->getOriginalXfer());
	T_ASC_PresentationContextID presID;

	if (filexfer.getXfer() != EXS_Unknown)
		presID = ASC_findAcceptedPresentationContextID(assoc, sopClass, filexfer.getXferID());
	else
		presID = ASC_findAcceptedPresentationContextID(assoc, sopClass);
	if (presID == 0) {
		cond = DIMSE_NOVALIDPRESENTATIONCONTEXTID;
		return cond.text();
	}

	T_DIMSE_C_StoreRQ req = {};
	req.MessageID = assoc->nextMsgID++;
	strcpy(req.AffectedSOPClassUID, sopClass);
	strcpy(req.AffectedSOPInstanceUID, sopInstance);
	req.DataSetType = DIMSE_DATASET_PRESENT;
	req.Priority = DIMSE_PRIORITY_LOW;

	T_DIMSE_C_StoreRSP rsp = {};
	DcmDataset* statusDetail = NULL;

	cond = DIMSE_storeUser(assoc, presID, &req, NULL, dcmff.getDataset(), NULL, NULL, opt_blockMode,
						   opt_dimseTimeout, &rsp, &statusDetail, nullptr, QFileInfo(fname).size());

	if (statusDetail) delete statusDetail;

	if (cond.bad())
		return cond.text();
	else
		return req.AffectedSOPInstanceUID;
}


OFCondition
Transfer::Imp::add_contexts()
{
	OFCondition cond;

	int pid = 1;
	for (int i = 0; i < numberOfDcmShortSCUStorageSOPClassUIDs; i++) {
		if (pid > 255) return ASC_BADPRESENTATIONCONTEXTID;

		char const* preferredSyntax = UID_LittleEndianExplicitTransferSyntax;
		cond = ASC_addPresentationContext(params, pid, dcmShortSCUStorageSOPClassUIDs[i],
										  &preferredSyntax, 1);
		if (cond.bad()) return cond;
		pid += 2;

		char const* fallbackSyntaxes[] = {UID_LittleEndianImplicitTransferSyntax};
		cond = ASC_addPresentationContext(params, pid, dcmShortSCUStorageSOPClassUIDs[i],
										  fallbackSyntaxes, countof(fallbackSyntaxes));
		if (cond.bad()) return cond;
		pid += 2;
	}

	return cond;
}
