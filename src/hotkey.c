#include <windows.h>
#include <stdio.h>

#define KEY_FROM VK_CAPITAL // key Caps lock (switch lang.)

// ===== Command Pattern =====
typedef struct {
    void (*execute)();
} KeyCommand;

void switchLanguage() {
    keybd_event(VK_CONTROL, 0, 0, 0);          // Ctrl down
    keybd_event(VK_SHIFT, 0, 0, 0);            // Shift down
    keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);  // Shift up
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); // Ctrl up
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