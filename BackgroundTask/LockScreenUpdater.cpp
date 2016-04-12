#include "pch.h"
#include "LockScreenUpdater.h"
#include <string>
#include <sstream>
#include <robuffer.h>

using namespace BackgroundTask;
using namespace concurrency;
using namespace Platform;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Markup;
using namespace Windows::UI::Xaml::Media::Imaging;

LockScreenUpdater::LockScreenUpdater()
{
}

//void LockScreenUpdater::OnRun(IBackgroundTaskInstance^ taskInstance)
//{
//	Agile<BackgroundTaskDeferral^> deferral = Agile<BackgroundTaskDeferral^>(taskInstance->GetDeferral());
//
//	if (UserProfilePersonalizationSettings::Current->IsSupported) {
//		create_task(StorageFile::GetFileFromApplicationUriAsync(ref new Uri("ms-appx:///Assets/lockScreen.xml"))).then([](StorageFile^ xamlFile) {
//			return FileIO::ReadTextAsync(xamlFile);
//		}).then([](String^ content) {
//			auto size = Window::Current->Bounds;
//
//			Grid^ root = (Grid^)XamlReader::Load(content);
//			root->Width = size.Width;
//			root->Height = size.Height;
//			for (auto ix = 0; ix < 4; ++ix) {
//
//				std::wstringstream imgName;
//				imgName << L"Img" << ix;
//				Image^ img = (Image^)root->FindName(ref new String(imgName.str().c_str()));
//
//				std::wstringstream path;
//				path << L"ms-appx:///Assets/" << ix << L".jpg";
//				img->Source = ref new BitmapImage(ref new Uri(ref new String(path.str().c_str())));
//
//				std::wstringstream tbName;
//				tbName << L"Tb" << ix;
//				TextBlock^ tb = (TextBlock^)root->FindName(ref new String(tbName.str().c_str()));
//
//				std::wstringstream tbText;
//				tbText << "text content" << ix;
//				tb->Text = ref new String(tbText.str().c_str());;
//			}
//
//			RenderTargetBitmap^ rtb = ref new RenderTargetBitmap();
//			return create_task(rtb->RenderAsync(root, size.Width, size.Height)).then([rtb]() {
//				return rtb->GetPixelsAsync();
//			}).then([rtb](IBuffer^ buffer) {
//				return create_task(ApplicationData::Current->LocalFolder->CreateFileAsync("lock-screen.jpg", CreationCollisionOption::ReplaceExisting)).then([buffer, rtb](StorageFile^ contentFile) {
//					return create_task(contentFile->OpenAsync(FileAccessMode::ReadWrite)).then([](IRandomAccessStream^ output) {
//						return BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId, output);
//					}).then([buffer, rtb](BitmapEncoder^ encoder) {
//
//						IUnknown* pUnk = reinterpret_cast<IUnknown*>(buffer);
//						IBufferByteAccess* pBufferByteAccess = nullptr;
//						pUnk->QueryInterface(IID_PPV_ARGS(&pBufferByteAccess));
//						byte *pixels = nullptr;
//						pBufferByteAccess->Buffer(&pixels);
//
//						Array<unsigned char>^ data = ref new Array<unsigned char>(pixels, rtb->PixelWidth * rtb->PixelHeight * 4);
//						encoder->SetPixelData(BitmapPixelFormat::Bgra8, BitmapAlphaMode::Premultiplied, rtb->PixelWidth, rtb->PixelHeight, 96, 96, data);
//
//						return encoder->FlushAsync();
//
//					}).then([contentFile]() {
//						return Windows::System::UserProfile::UserProfilePersonalizationSettings::Current->TrySetLockScreenImageAsync(contentFile);
//					});
//				});
//			});
//
//
//		}).then([deferral](bool ok) {
//			deferral->Complete();
//		});
//	} else {
//		deferral->Complete();
//	}
//}

void LockScreenUpdater::OnRun(IBackgroundTaskInstance^ taskInstance)
{
	Agile<BackgroundTaskDeferral^> deferral = Agile<BackgroundTaskDeferral^>(taskInstance->GetDeferral());

	if (UserProfilePersonalizationSettings::Current->IsSupported) {

		create_task(StorageFile::GetFileFromApplicationUriAsync(ref new Uri("ms-appx:///Assets/lockScreen.xml"))).then([](StorageFile^ xamlFile) {
			return FileIO::ReadTextAsync(xamlFile);
		}).then([](String^ content) {
			auto size = Window::Current->Bounds;

			Grid^ root = (Grid^)XamlReader::Load(content);
			root->Width = size.Width;
			root->Height = size.Height;
			for (auto ix = 0; ix < 4; ++ix) {
				
				std::wstringstream imgName;
				imgName << L"Img" << ix;
				Image^ img = (Image^)root->FindName(ref new String(imgName.str().c_str()));
				
				std::wstringstream path;
				path << L"ms-appx:///Assets/" << ix << L".jpg";
				img->Source = ref new BitmapImage(ref new Uri(ref new String(path.str().c_str())));
				
				std::wstringstream tbName;
				tbName << L"Tb" << ix;
				TextBlock^ tb = (TextBlock^)root->FindName(ref new String(tbName.str().c_str()));

				std::wstringstream tbText;
				tbText << "text content" << ix;
				tb->Text = ref new String(tbText.str().c_str());;
			}

			RenderTargetBitmap^ rtb = ref new RenderTargetBitmap();
			return create_task(rtb->RenderAsync(root, size.Width, size.Height)).then([rtb]() {
				return rtb->GetPixelsAsync();
			}).then([rtb](IBuffer^ buffer) {

				return create_task(ApplicationData::Current->LocalFolder->CreateFileAsync("lock-screen.jpg", CreationCollisionOption::ReplaceExisting)).then([buffer, rtb](StorageFile^ contentFile) {
					return create_task(contentFile->OpenAsync(FileAccessMode::ReadWrite)).then([](IRandomAccessStream^ output) {
						return BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId, output);
					}).then([buffer, rtb](BitmapEncoder^ encoder) {

						IUnknown* pUnk = reinterpret_cast<IUnknown*>(buffer);
						IBufferByteAccess* pBufferByteAccess = nullptr;
						pUnk->QueryInterface(IID_PPV_ARGS(&pBufferByteAccess));
						byte *pixels = nullptr;
						pBufferByteAccess->Buffer(&pixels);

						Array<unsigned char>^ data = ref new Array<unsigned char>(pixels, rtb->PixelWidth * rtb->PixelHeight * 4);
						encoder->SetPixelData(BitmapPixelFormat::Bgra8, BitmapAlphaMode::Premultiplied, rtb->PixelWidth, rtb->PixelHeight, 96, 96, data);
						
						return encoder->FlushAsync();
					}).then([contentFile]() {
						return Windows::System::UserProfile::UserProfilePersonalizationSettings::Current->TrySetLockScreenImageAsync(contentFile);
					});
				});
			});
		}).then([deferral](bool ok) {
			deferral->Complete();
		});

	} else {
		// todo ...
		deferral->Complete();
	}

}
