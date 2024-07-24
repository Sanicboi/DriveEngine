#include "Main.h"
#include "DXGIContext.h"
#include "Device.h"
#include "CommandQueue.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prev, PWSTR cmdLine, int nCmdShow) 
{
    OutputDebugStringA("Hello world!\n");
    DXGIContext context;
    Device device(context.Adapter());
    CommandQueue queue(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
    return 0;
}