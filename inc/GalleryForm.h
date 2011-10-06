#ifndef _GALLERYFORM_H_
#define _GALLERYFORM_H_

#include <FGraphics.h>
#include <FUi.h>
#include <FUiControl.h>
#include <FBase.h>
#include <FMedia.h>

class GalleryForm: public Osp::Ui::Controls::Form,
		public Osp::Ui::IOrientationEventListener,
		public Osp::Ui::IActionEventListener,
		public Osp::Ui::Controls::IGalleryEventListener,
		public Osp::Ui::Controls::IGalleryItemProvider

{
private:

	void ConfigureSlideShowButton();
	void ConfigureTitle();
	void ConfigureDataSource();
	void DestroyDataSource();

	Osp::Ui::Controls::Gallery * pGallery;
	Osp::Base::Collection::ArrayList * pImagesPaths;

	/* Action ids */
	static const int ACTION_ID_TOGGLE_SLIDESHOW = 101;

public:
	GalleryForm(void);
	virtual ~GalleryForm(void);
	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	/* IActionEventListener */
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	/* IOrientationEventListener */
	virtual void OnOrientationChanged(const Osp::Ui::Control& source, Osp::Ui::OrientationStatus orientationStatus);

	/* IGalleryEventListener */
	virtual void OnGalleryCurrentItemChanged(Osp::Ui::Controls::Gallery &gallery, int index);
	virtual void OnGalleryItemClicked(Osp::Ui::Controls::Gallery &gallery, int index);
	virtual void OnGallerySlideShowStarted(Osp::Ui::Controls::Gallery& gallery);
	virtual void OnGallerySlideShowStopped(Osp::Ui::Controls::Gallery& gallery);

	/* IGalleryItemProvider */
	virtual Osp::Ui::Controls::GalleryItem* CreateItem(int index);
	virtual bool DeleteItem(int index, Osp::Ui::Controls::GalleryItem *pItem);
	virtual int GetItemCount(void);

};

#endif	//_GALLERYFORM_H_
