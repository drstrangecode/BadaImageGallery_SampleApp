#include "GalleryForm.h"

using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Media;

GalleryForm::GalleryForm(void) {
	pImagesPaths = null;
}

GalleryForm::~GalleryForm(void) {
}

bool GalleryForm::Initialize() {
	Construct(L"IDF_GALLERYFORM");
	return true;
}

void GalleryForm::ConfigureSlideShowButton() {

	FooterItem * newFooterItem = new FooterItem();
	newFooterItem->Construct(ACTION_ID_TOGGLE_SLIDESHOW);

	if (pGallery->IsSlideShowStarted()) {
		newFooterItem->SetText(L"Stop Slideshow");
	}
	else {
		newFooterItem->SetText(L"Start Slideshow");
	}

	GetFooter()->RemoveItemAt(0);
	GetFooter()->AddItem(*newFooterItem);

	delete newFooterItem;

}

void GalleryForm::ConfigureTitle() {

	String newTitle;

	newTitle.Append(pGallery->GetCurrentItemIndex() + 1);
	newTitle.Append(L"/");
	newTitle.Append(pImagesPaths->GetCount());

	GetHeader()->SetTitleText(newTitle);

}

void GalleryForm::DestroyDataSource() {
	if (pImagesPaths != null) {
		pImagesPaths->RemoveAll(true);
		delete pImagesPaths;
	}
}

void GalleryForm::ConfigureDataSource() {

	DestroyDataSource();

	pImagesPaths = new ArrayList();
	pImagesPaths->Construct();

	if (GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT ||
		GetOrientationStatus() == ORIENTATION_STATUS_PORTRAIT_REVERSE) {
		pImagesPaths->Add(*new String(L"/Home/portrait_1.jpg"));
		pImagesPaths->Add(*new String(L"/Home/portrait_2.jpg"));
		pImagesPaths->Add(*new String(L"/Home/portrait_3.jpg"));
	}
	else {
		pImagesPaths->Add(*new String(L"/Home/landscape_1.jpg"));
		pImagesPaths->Add(*new String(L"/Home/landscape_2.jpg"));
		pImagesPaths->Add(*new String(L"/Home/landscape_3.jpg"));
		pImagesPaths->Add(*new String(L"/Home/landscape_4.jpg"));
		pImagesPaths->Add(*new String(L"/Home/landscape_5.jpg"));
	}
}

result GalleryForm::OnInitializing(void) {
	result r = E_SUCCESS;

	/* Add the orientation event listener to
	 * receive an event when the phone is rotated */
	AddOrientationEventListener(*this);

	/* Enable this form to receive events from
	 * the footer button by adding itself as the
	 * event listener on the Footer */
	GetFooter()->AddActionEventListener(*this);

	pGallery = static_cast<Gallery *> (GetControl(L"IDC_GALLERY"));
	pGallery->SetItemProvider(*this);
	pGallery->SetCurrentItemIndex(0);
	pGallery->AddGalleryEventListener(*this);

	ConfigureDataSource();
	ConfigureSlideShowButton();
	ConfigureTitle();

	return r;
}

result GalleryForm::OnTerminating(void) {
	result r = E_SUCCESS;

	DestroyDataSource();

	return r;
}

void GalleryForm::OnOrientationChanged(const Control& source, OrientationStatus orientationStatus) {

	ConfigureDataSource();

	pGallery->StopSlideShow();
	pGallery->UpdateGallery();
	pGallery->SetCurrentItemIndex(0);

	ConfigureTitle();

	RequestRedraw();
}

void GalleryForm::OnActionPerformed(const Control& source, int actionId) {

	switch (actionId) {
	case ACTION_ID_TOGGLE_SLIDESHOW:
		if (pGallery->IsSlideShowStarted()) {
			pGallery->StopSlideShow();
		}
		else {
			/* Configure the animation type */
			pGallery->SetSlideShowAnimation(GALLERY_ANIMATION_PAGE);

			/* Configure the animation duration between different images */
			pGallery->SetSlideShowAnimationDuration(3000);

			/* Configure the duration of each image */
			pGallery->SetSlideShowViewDuration(2000);

			/* Start the slideshow, pass true to enable repeat */
			pGallery->StartSlideShow(true);
		}
		break;
	default:
		break;
	}

	ConfigureSlideShowButton();
	RequestRedraw();

}

// IGalleryEventListener
void GalleryForm::OnGalleryCurrentItemChanged(Osp::Ui::Controls::Gallery &gallery, int index) {

	ConfigureTitle();
	RequestRedraw();

}

void GalleryForm::OnGalleryItemClicked(Osp::Ui::Controls::Gallery &gallery, int index) {

	 String *imagePath = static_cast<String *>(pImagesPaths->GetAt(index));

	 MessageBox messageBox;
	 messageBox.Construct(L"Image details", *imagePath, MSGBOX_STYLE_NONE, 3000);

	 int modalResult = 0;
	 messageBox.ShowAndWait(modalResult);

}

void GalleryForm::OnGallerySlideShowStarted(Osp::Ui::Controls::Gallery& gallery) {
	ConfigureSlideShowButton();
	Draw();
	Show();
}

void GalleryForm::OnGallerySlideShowStopped(Osp::Ui::Controls::Gallery& gallery) {
	ConfigureSlideShowButton();
	Draw();
	Show();
}

//IGalleryItemProvider
Osp::Ui::Controls::GalleryItem* GalleryForm::CreateItem(int index) {

	String *imagePath = static_cast<String *>(pImagesPaths->GetAt(index));

	Image* pImage = new Image();
	pImage->Construct();
	Bitmap * pBitmap = pImage->DecodeN(*imagePath, BITMAP_PIXEL_FORMAT_ARGB8888);
	delete pImage;

	GalleryItem* pGalleryItem = new GalleryItem();
	pGalleryItem->Construct(*pBitmap, *imagePath);

	delete pBitmap;

	return pGalleryItem;
}

bool GalleryForm::DeleteItem(int index, Osp::Ui::Controls::GalleryItem *pItem) {
	delete pItem;
	return true;
}

int GalleryForm::GetItemCount(void) {
	return pImagesPaths->GetCount();
}

