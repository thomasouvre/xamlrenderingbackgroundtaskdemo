#pragma once

namespace BackgroundTask
{
	public ref class LockScreenUpdater sealed : Windows::UI::Xaml::Media::Imaging::XamlRenderingBackgroundTask
	{
	private:
	protected:
		void OnRun(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ taskInstance) override;
	public:
		LockScreenUpdater();
	};
}
