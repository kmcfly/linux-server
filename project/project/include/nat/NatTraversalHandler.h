// NatTraversalHandler.h: interface for the class CNatTraversalHandler.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_TRAVERSAL_HANDLER_H
#define _NAT_TRAVERSAL_HANDLER_H

#include "NatSimpleDataBlock.h"
#include "NatTraversalXml.h"

class CNatTraversalHandler: public CNatDataBlockParser::CParserNotifier,
    CNatDataBlockSender::CSenderNotifier
{
public:
    class CHandlerNotifier
    {
    public:
        /**
         * �������ݻص�֪ͨ
         * ����Ҫ���͵Ĵ�͸���ת������Ϊ�ɷ��͵����ݺ󣬻ص�֪ͨ��
         *
         */
        virtual int OnSendTraversalData(const char* pData, int iLen) = 0;

        /**
         * �ڽ�������ǰ�ص�֪ͨ
		 * ��Ҫ�Ǹ�������ѡ���Ƿ��������ص�֪ͨ�����
		 * @param[in] version �汾��
		 * @param[in] cmdId ����ID
         * @param[in] isHandled �Ƿ�������Ĭ����true���������Ϊfalse�������Ը�����Ĵ���
         */
        virtual void OnBeforeRecvCmd(const NAT_TRAVERSAL_VERSION *version, const NatTraversalCmdId &cmdId, bool &isHandled) = 0;

        /**
		* ��������ص�֪ͨ
		* @param[in] version �汾��
         * @param[in] cmdId ����ID
         * @param[in] cmd �����������
         */
        virtual void OnRecvCmd(const NAT_TRAVERSAL_VERSION* version, const NatTraversalCmdId &cmdId, void* cmd) = 0;

		/**
		 * �����������
		 * ��Ҫ�ǽ�������
		 * @param[in] error ��������
		 */
		virtual void OnRecvCmdError(CNatTraversalXmlParser::ParseError error) = 0;
    };
public:
    CNatTraversalHandler();

    ~CNatTraversalHandler();

    bool Initialize();

    bool Initialize(CNatTraversalXmlPacker *xmlPacker, CNatTraversalXmlParser *xmlParser);

    void Finalize();

    bool IsInitialized();

    bool RecvTraversalData(const char* pData, int iLen);

	bool SendCmd(NatTraversalCmdId cmdId, void *cmd, int cmdLen);

	bool SendCmd(NatTraversalCmdId cmdId, void *cmd, int cmdLen, const NAT_TRAVERSAL_VERSION* version);

    NatRunResult RunSend();

    bool IsSending();

    void SetNotifier(CHandlerNotifier *notifier);
public:
    ////////////////////////////////////////////////////////////////////////////////
    // CNatDataBlockParser::CNotifier public interface

    virtual int OnRecvDataBlock(CNatDataBlockParser *parser, const char* pDataBlock, 
        int iLen);
public:
    ////////////////////////////////////////////////////////////////////////////////
    // CNatDataBlockSender::CSenderNotifier public interface

    virtual int OnSendDataBlock(CNatDataBlockSender *sender, const char* pDataBlock, 
        int iLen);
private:
    void Clear();
private:
    CNatTraversalXmlPacker *m_xmlPacker;
    CNatTraversalXmlParser *m_xmlParser;
    CNatTraversalXmlPacker *m_ownedXmlPacker;
    CNatTraversalXmlParser *m_ownedXmlParser;
    CNatDataBlockParser m_dataBlockParser;
    CNatDataBlockSender m_dataBlockSender;
    CHandlerNotifier *m_notifier;
    bool m_isInitialized;
};

#endif//_NAT_TRAVERSAL_HANDLER_H