#include <windows.h>
#include <stdio.h>

#define KEY_FROM VK_CAPITAL // key Caps lock (switch lang.)

// ===== Command Pattern =====
typedef struct {
    void (*execute)();
} KeyCommand;

void switchLanguage() {
    INPUT inputs[4] = {0};

    // Ctrl down
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    // Shift down
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_SHIFT;

    // Shift up
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_SHIFT;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    // Ctrl up
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
}

KeyCommand createCapLockCommand() {
    KeyCommand cmd;
    cmd.execute = switchLanguage;
    return cmd;
}

// ===== Strategy Pattern =====
typedef struct {
    BOOL (*shouldRemap)(DWORD vkCode);
    KeyCommand* (*getCommand)(DWORD vkCode);
} KeyRemapper;

BOOL isRemapTarget(DWORD vkCode) {
    return vkCode == KEY_FROM;
}

KeyCommand* getRemapCommand(DWORD vkCode) {
    static KeyCommand capsCmd = { .execute = switchLanguage };
    static KeyCommand noop    = { .execute = NULL };

    if (vkCode == KEY_FROM) {
        return &capsCmd;
    }

    return &noop;
}

KeyRemapper createDefaultRemapper() {
    KeyRemapper r;
    r.shouldRemap = isRemapTarget;
    r.getCommand = getRemapCommand;
    return r;
}

// ===== Hook System =====

HHOOK hHook;
KeyRemapper remapper;

LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *) lParam;
        if (remapper.shouldRemap(p -> vkCode)) {
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
                KeyCommand* cmd = remapper.getCommand(p -> vkCode);
                if (cmd -> execute != NULL) {
                    cmd->execute();
                }
            }
            return 1;
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {

    HANDLE hMutex = CreateMutex(NULL, TRUE, "LangSwitcherSingleton");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return 0; 
    }

    
    HKL hklThai = LoadKeyboardLayout("0000041E", KLF_ACTIVATE);
    if (!hklThai) {
        MessageBox(NULL, "Failed to load Thai keyboard layout", "Error", MB_OK);
        return 1;
    }
    ActivateKeyboardLayout(hklThai, KLF_SETFORPROCESS);
    
    MSG msg;
    remapper = createDefaultRemapper();
    
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, NULL, 0);
    if (!hHook) {
        // Error hHook
        return 1;
    }

    while (GetMessage(&msg, NULL, 0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    return 0;
}