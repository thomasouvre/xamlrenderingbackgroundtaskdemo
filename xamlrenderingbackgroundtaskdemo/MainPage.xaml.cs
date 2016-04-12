using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Windows.ApplicationModel.LockScreen;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.System.UserProfile;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace xamlrenderingbackgroundtaskdemo
{
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private async Task RegisterBackgroundTasks()
        {
            foreach (var task in BackgroundTaskRegistration.AllTasks)
            {
                task.Value.Unregister(true);
            }

            var result = await BackgroundExecutionManager.RequestAccessAsync();
            if (result == BackgroundAccessStatus.Denied)
            {
                return;
            }

            BackgroundTaskBuilder builder = new BackgroundTaskBuilder();
            builder.Name = "LockScreenUpdater";
            builder.TaskEntryPoint = "BackgroundTask.LockScreenUpdater";

            builder.SetTrigger(new SystemTrigger(SystemTriggerType.NetworkStateChange, false));
            var registration = builder.Register();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            RegisterBackgroundTasks();
        }
    }
}
