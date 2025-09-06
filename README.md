# 🔤 Hotkey Language Switcher

โปรแกรมสำหรับ remap ปุ่ม `Caps Lock` ให้กลายเป็นตัวเปลี่ยนภาษา (แทนปุ่ม Grave Accent `)  
ใช้งานเบา ทำงานอัตโนมัติทุกครั้งหลังเปิดเครื่อง

---

## 🧱 Registry Hives ที่เกี่ยวข้อง

| Hive Key | ชื่อเต็ม | ใช้ทำอะไร |
|----------|----------|-------------|
| `HKCR`   | HKEY_CLASSES_ROOT   | ไฟล์สัมพันธ์, COM Class |
| `HKCU`   | HKEY_CURRENT_USER   | การตั้งค่าผู้ใช้ปัจจุบัน เช่น startup |
| `HKLM`   | HKEY_LOCAL_MACHINE  | การตั้งค่าระดับระบบ |
| `HKU`    | HKEY_USERS           | ผู้ใช้ทั้งหมด |
| `HKCC`   | HKEY_CURRENT_CONFIG | โปรไฟล์ฮาร์ดแวร์ปัจจุบัน |

---

## 🛠️ วิธีติดตั้งจาก Source Code

```bash
gcc .\hotkey.c -o HotkeyLanguageSwitcher.exe -mwindows
```
## 🚀 ขั้นตอนการติดตั้ง 
หลังจากเราได้ไฟล์ HotkeyLanguageSwitcher.exe ให้เราสร้าง Path ที่เราต้องแล้วทำการ Copy Path ดังกล่าวตามตัวอย่างดังนี้

> "C:\\Path\\To\\HotkeyLanguageSwitcher.exe\"  

เราสามารถเลือกวิธีการติดตั้งได้สองรูปแบบคือติดตั้งผ่าน `installer/config` หรือ ติดตั้งด้วยตัวเอง

```bash
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" ^
 /v "HotkeyLanguageSwitcher" ^
 /t REG_SZ ^
 /d "\"C:\\Path\\To\\HotkeyLanguageSwitcher.exe\"" ^
 /f
```  
หรือสามารถนำไปแก้ไขปรับแต่งเองได้ตาม Syntax :
> reg add <keyname> [/v valuename | /ve] [/t datatype] [/s separator] [/d data] [/f] [/reg:32 | /reg:64]

## 🧹 ขั้นตอนการลบ Reg
เราสามารถเข้าไปยัง `installer/` เราจะพบไฟล์ reset.bat 
## 🚀 เป้าหมายที่คาดหวัง
ฟังก์ชันชวเลข (Stenotype)
## 💡ฟังก์ชันที่จะทำเพิ่มเติม
- ติดตั้งอัตโนมัติ
- ความคล่องตัวของฟังก์ชัน เนื่องจากในงานยังคงมีการกำหนดทุกอย่างจึงอาจใช้งานยุ่งยาก
