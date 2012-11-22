#ifndef __NOTIFY__MSG__
#define	__NOTIFY__MSG__

// notify��Ϣ����
enum
{
	OT_MSG_START = 0,

	OT_MSG_REPAINT,				 /**< �ػ�GL���� */
	OT_MSG_COMPLETE_FRAME,		 /**< ���һ֡�Ļ��ƣ���glflush/glfinish/glswapbuffer����� */
	OT_MSG_TIMER_START,			 /**< ��ʱ�����ü����� */
	OT_MSG_TEST_BEGIN,			 /**< ���Կ�ʼ --ÿ�����Ժ�����ʼ����Ҫ����*/
	OT_MSG_TEST_FINISH,			 /**< ���Խ��� --ÿ�����Ժ�����������Ҫ���� */


	OT_MSG_END
};

// notify�ص���������
typedef void (*FnNotify)(int nMsgID,void *pParam1,void* pParam2);

#endif	//__NOTIFY__MSG__