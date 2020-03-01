#include <Uefi.h> 
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>  //Provide "EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *SimpleFileSystem" function

//UefiMain
VOID FunctionSelectMenu(VOID);
CHAR16 GetFunctionSelectResult(VOID);
//GetUnicodeChar
CHAR16 GetUnicodeChar(VOID);

//OpenFile
EFI_STATUS OpenFile(EFI_FILE_PROTOCOL **ReadMe);

//CopyFile
VOID CopyFile();

//WriteFile
EFI_STATUS WriteFile(VOID);

//ReadFile
EFI_STATUS ReadFile(VOID);

//DeleteFile
EFI_STATUS DeleteFile(VOID);

//MergeFile
VOID MergeFile(VOID);


EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS   Status;
  CHAR16       FunctionSelectResult;
  
  FunctionSelectMenu();
  while((FunctionSelectResult = GetFunctionSelectResult()) != 6)
  {
    if(FunctionSelectResult == 1)
      CopyFile();
    else if(FunctionSelectResult == 2)
      Status = WriteFile();
    else if(FunctionSelectResult == 3)
      Status = ReadFile();
    else if(FunctionSelectResult == 4)
      Status = DeleteFile();
    else if(FunctionSelectResult == 5)
      MergeFile();
    else
      Print(L"\nError Select!!! Please Select again\n");
    FunctionSelectMenu();
  }
  
  return EFI_SUCCESS;
}
//FunctionSelectMenu
VOID FunctionSelectMenu(VOID)
{
  Print(L"\n<<FunctionSelectMenu>>\n");
  Print(L"1. Copy a file\n");
  Print(L"2. Write a file\n");
  Print(L"3. Read a file\n");
  Print(L"4. Delete two file data\n");
  Print(L"5. Merge two files\n");
  Print(L"6. Quit the program\n");
  Print(L"Please select the function: ");
}
CHAR16 GetFunctionSelectResult(VOID)
{ 
  INT8 i = 0;
  CHAR16 FunctionSelectResult[3];
  
  while((FunctionSelectResult[i] = GetUnicodeChar()) != 13)
  {
    Print(L"%c", FunctionSelectResult[i]);
    i++;
  }
  FunctionSelectResult[0] = FunctionSelectResult[0] - '0';
  return FunctionSelectResult[0];
}
//GetUnicodeChar
CHAR16 GetUnicodeChar(VOID)
{
  UINTN Index;
  EFI_INPUT_KEY Key;
  CHAR16 KeyUnicodeChar;
  
  gBS -> WaitForEvent(1, &gST -> ConIn -> WaitForKey, &Index);
  gST -> ConIn -> ReadKeyStroke(gST -> ConIn, &Key);
  KeyUnicodeChar = Key.UnicodeChar;
    
  return KeyUnicodeChar;
}


//OpenFile
EFI_STATUS OpenFile(EFI_FILE_PROTOCOL **ReadMe)
{
	EFI_STATUS                        Status = 0;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *SimpleFileSystem;
  EFI_FILE_PROTOCOL                 *Root = 0;
  
  
  Status = gBS -> LocateProtocol(&gEfiSimpleFileSystemProtocolGuid, NULL, (VOID**)&SimpleFileSystem);
  Status = SimpleFileSystem -> OpenVolume(SimpleFileSystem, &Root);
        
  Status = Root -> Open(Root, ReadMe, (CHAR16*)L"ReadMe.txt", EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,0);
  
  return Status;
}


//CopyFile
VOID CopyFile(VOID)
{
  Print(L"\n>>Copy a File(wait to improve)\n");
}


//WriteFile
EFI_STATUS WriteFile(VOID)
{
	EFI_STATUS          Status = 0;
  UINTN               BufSize;
  CHAR16              *Buf= (CHAR16*)L"This is test file\n";
//  EFI_FILE_PROTOCOL   *Root = 0;
  EFI_FILE_PROTOCOL   *ReadMe = 0;
  
  Print(L"\n>>Write a file(default: ReadMe.txt)\n");
  Status = OpenFile(&ReadMe);  
  if( ReadMe && !EFI_ERROR(Status))
  {
    BufSize = StrLen(Buf) * 2;
    Status = ReadMe -> Write (ReadMe, &BufSize, Buf);  
    Status = ReadMe -> Close(ReadMe);
  }

  return Status;
}


//ReadFile
EFI_STATUS ReadFile(VOID)
{
	EFI_STATUS          Status = 0;
  EFI_FILE_PROTOCOL   *ReadMe = 0;
  CHAR16 Buf[65];
  UINTN BufSize = 64;
  
  Print(L"\n>>Read a file(default: ReadMe.txt)\n");
  Status = OpenFile(&ReadMe);
  Status = ReadMe -> Read( ReadMe, &BufSize, Buf);
  if(!EFI_ERROR(Status))
  {
    Buf[BufSize] = 0;
    Print(L"%s\n", Buf);
  }
  Status = ReadMe -> Close(ReadMe);
    
  return Status;
}


//DeleteFile
EFI_STATUS DeleteFile(VOID)
{
  UINTN               Status;
  EFI_FILE_PROTOCOL   *ReadMe;
  
  Print(L"\n>>Delete a file(default: ReadMe.txt)\n");
  Status = OpenFile(&ReadMe);
  Status = ReadMe -> Delete(ReadMe);
  
  return Status;
}


//MergeFile
VOID MergeFile(VOID)
{ 
  Print(L">>\nMerge two files(wait to improve)\n");
}























