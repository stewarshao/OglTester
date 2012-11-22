#ifndef UIFRAME_GLWIDGET_MANAGER
#define UIFRAME_GLWIDGET_MANAGER

#include <map>
using namespace std;

class QString;
class QDockWidget;
class CScene;

#include "globaldef.h"

class CGlWidgetManager
{
public:
	static CGlWidgetManager* Instance();

	static void destroy();

	int init();

	void deinit();

	/************************************************************************** 
	* @brief NewGlWidget 
	* 
	* 创建新的gl窗口.
	* @param[in] pNewWidget	新tab页的指针
	* @param[in] pScene		场景指针
	* @return GlwHandle		成功 gl窗口句柄
	*						失败 unsigned(-1)
	**************************************************************************/
	GlwHandle NewGlWidget(QDockWidget*& pNewWidget,CScene* pScene);

	void DeleteGlWidget(GlwHandle index);


	/************************************************************************** 
	* @brief Grab 
	* 
	* 抓图.
	* @param[in] FileRoot			抓图存放目录
	* @return int					成功 0
	*								失败 其他错误码
	**************************************************************************/
	int Grab(int index, QString FileRoot);

	int FullScreen(int index,bool bF = true);

private:
	CGlWidgetManager();
	~CGlWidgetManager();

	static CGlWidgetManager* m_pMan;

	map<int,QDockWidget*> m_mapGlWidget;	/**< 使用map管理glwidget */

	GlwHandle m_nNextGlHandle;				/**< 下一个窗口的句柄值，当窗口销毁时，此值不减 */

	bool m_bFullGlExist;					 /**< 是否已有一个gl处于全屏状态 */
};

#endif //UIFRAME_GLWIDGET_MANAGER