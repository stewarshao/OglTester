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
	* �����µ�gl����.
	* @param[in] pNewWidget	��tabҳ��ָ��
	* @param[in] pScene		����ָ��
	* @return GlwHandle		�ɹ� gl���ھ��
	*						ʧ�� unsigned(-1)
	**************************************************************************/
	GlwHandle NewGlWidget(QDockWidget*& pNewWidget,CScene* pScene);

	void DeleteGlWidget(GlwHandle index);


	/************************************************************************** 
	* @brief Grab 
	* 
	* ץͼ.
	* @param[in] FileRoot			ץͼ���Ŀ¼
	* @return int					�ɹ� 0
	*								ʧ�� ����������
	**************************************************************************/
	int Grab(int index, QString FileRoot);

	int FullScreen(int index,bool bF = true);

private:
	CGlWidgetManager();
	~CGlWidgetManager();

	static CGlWidgetManager* m_pMan;

	map<int,QDockWidget*> m_mapGlWidget;	/**< ʹ��map����glwidget */

	GlwHandle m_nNextGlHandle;				/**< ��һ�����ڵľ��ֵ������������ʱ����ֵ���� */

	bool m_bFullGlExist;					 /**< �Ƿ�����һ��gl����ȫ��״̬ */
};

#endif //UIFRAME_GLWIDGET_MANAGER