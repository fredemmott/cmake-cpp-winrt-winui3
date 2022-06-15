#pragma once

#include "pch.h"
#include "MainWindow.g.h"

namespace winrt::DemoApp::implementation {
struct MainWindow : MainWindowT<MainWindow> {
  MainWindow();
};
}

namespace winrt::DemoApp::factory_implementation {
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow> {};
}

