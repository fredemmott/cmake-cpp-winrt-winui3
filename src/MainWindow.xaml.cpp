#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

namespace winrt::DemoApp::implementation {

MainWindow::MainWindow() {
  InitializeComponent();
}

}
