#ifndef __NOTIFY__MSG__
#define	__NOTIFY__MSG__

// notify消息定义
enum
{
	OT_MSG_START = 0,

	OT_MSG_REPAINT,				 /**< 重回GL窗口 */
	OT_MSG_COMPLETE_FRAME,		 /**< 完成一帧的绘制，在glflush/glfinish/glswapbuffer后调用 */
	OT_MSG_TIMER_START,			 /**< 定时器设置及开启 */
	OT_MSG_TEST_BEGIN,			 /**< 测试开始 --每个测试函数开始都需要发送*/
	OT_MSG_TEST_FINISH,			 /**< 测试结束 --每个测试函数结束都需要发送 */


	OT_MSG_END
};

// notify回调函数定义
typedef void (*FnNotify)(int nMsgID,void *pParam1,void* pParam2);

#endif	//__NOTIFY__MSG__