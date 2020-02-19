#include <Uefi.h> 
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/DiskIo.h>//Provide protocol name: gEfiDiskIoProtocolGuid
//UefiMain
VOID FunctionSelectMenu(VOID);
CHAR16 GetFunctionSelectResult(VOID);
//GetUnicodeChar
CHAR16 GetUnicodeChar(VOID);
//AllHandle
EFI_STATUS AllHandle(VOID);
EFI_STATUS ProtocolList(UINTN HandleAmount, EFI_HANDLE *HandleArray);
//ProtocolGuidHandle
EFI_STATUS ProtocolGuidHandle(VOID);
EFI_GUID GetProtocolGuid(VOID);
//ProtocolNameHandle
EFI_STATUS ProtocolNameHandle(VOID);
//HandleNumberHandle
EFI_STATUS HandleNumberHandle(VOID);
UINTN GetHandleNumber(VOID);

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS   Status;
  CHAR16       FunctionSelectResult;
  
  FunctionSelectMenu();
  while((FunctionSelectResult = GetFunctionSelectResult()) != 5)
  {
    if(FunctionSelectResult == 1)
      Status = AllHandle();
    else if(FunctionSelectResult == 2)
      Status = ProtocolGuidHandle();
    else if(FunctionSelectResult == 3)
      Status = ProtocolNameHandle();
    else if(FunctionSelectResult == 4)
      Status = HandleNumberHandle();
    else
      Print(L"\nError Select!!! Please Select again\n");
    FunctionSelectMenu();
  }
 
  return EFI_SUCCESS;
}
//VOID FunctionSelectMenu(VOID)
VOID FunctionSelectMenu(VOID)
{
  Print(L"\n<<FunctionSelectMenu>>\n");
  Print(L"1. Search all handles\n");
  Print(L"2. Search specific handles by protocol GUID\n");
  Print(L"3. Search specific handles by protocol name\n");
  Print(L"4. Search specific handles by handle number\n");
  Print(L"5. Quit program\n");
  Print(L"Please select the function: ");
}
//CHAR16 GetFunctionSelectResult(VOID)
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


//AllHandle
EFI_STATUS AllHandle(VOID)
{
  EFI_STATUS   Status;
  //LocateHandleBuffer
  UINTN        HandleAmount;
  EFI_HANDLE   *HandleArray;
  
  Status = gBS -> LocateHandleBuffer(AllHandles, NULL, NULL, &HandleAmount, &HandleArray);
  Status = ProtocolList(HandleAmount, HandleArray);
  
  return Status;
}
//ProtocolList
EFI_STATUS ProtocolList(UINTN HandleAmount, EFI_HANDLE *HandleArray)
{
  UINTN         i = 0;
  UINTN         j = 0;
  UINTN         k = 0;
  EFI_STATUS   Status = 0;
  //ProtocolsPerHandle
  EFI_GUID     **ProtocolArray;
  UINTN        ProtocolAmount;
  
  for(i = 0; i < HandleAmount; i++)
  {
    Print(L"\nHandle[%d] (address: 0x%p)\n", i, HandleArray[i]);
    Status = gBS -> ProtocolsPerHandle(HandleArray[i], &ProtocolArray, &ProtocolAmount);
    for(j = 0; j < ProtocolAmount; j++)
    {
      Print(L"Protocol[%d]: %08X-%04X-", j, ProtocolArray[j] -> Data1, ProtocolArray[j] -> Data2);
      Print(L"%04X-", ProtocolArray[j] -> Data3);
      for(k = 0; k < 8; k++)
        Print(L"%02X",  ProtocolArray[j] -> Data4[k]);
      Print(L"\n");
    }
  }
  return Status;  
}


//ProtocolNameHandle
EFI_STATUS ProtocolNameHandle(VOID)
{
  EFI_STATUS   Status;
  //LocateHandleBuffer
  UINTN        HandleAmount;
  EFI_HANDLE   *HandleArray;
  
  Print(L"\nProtocolName: gEfiDiskIoProtocolGuid");
  Status = gBS -> LocateHandleBuffer(ByProtocol, &gEfiDiskIoProtocolGuid, NULL, &HandleAmount, &HandleArray);
  Status = ProtocolList(HandleAmount, HandleArray);
    
  return Status;
}


//ProtocolGuidHandle
EFI_STATUS ProtocolGuidHandle(VOID)
{
  EFI_STATUS   Status;
  //LocateHandleBuffer
  EFI_GUID     ProtocolGuid;
  UINTN        HandleAmount;
  EFI_HANDLE   *HandleArray;
  
  ProtocolGuid = GetProtocolGuid();
  Status = gBS -> LocateHandleBuffer(ByProtocol, &ProtocolGuid, NULL, &HandleAmount, &HandleArray);
  Status = ProtocolList(HandleAmount, HandleArray);
  
  return Status;
}
//GetProtocolGuid
EFI_GUID GetProtocolGuid(VOID)
{
  UINT8    Temp[33];
  EFI_GUID  ProtocolGuid;  
  UINT8    i = 0;
  UINT8    j = 0;
  
  ProtocolGuid.Data1 = 0;
  ProtocolGuid.Data2 = 0;
  ProtocolGuid.Data3 = 0;
  for(j = 0; j < 8; j++)
    ProtocolGuid.Data4[j] = 0;
  
  Print(L"\nGUID: ");
  while((Temp[i] = (UINT8)GetUnicodeChar()) != 13)
  {
    Print(L"%c", Temp[i]);
    if(i == 7)
      Print(L"-");
    else if(i == 11)
      Print(L"-");
    else if(i == 15)
      Print(L"-");
    
    if('0' <= Temp[i] && Temp[i] <= '9')
      Temp[i] = Temp[i] - '0';
    else if('A' <= Temp[i] && Temp[i] <= 'F')
      Temp[i] = Temp[i] - 'A' + 10;
    else if('a' <= Temp[i] && Temp[i] <= 'f')
      Temp[i] = Temp[i] - 'a' + 10;
    
    if(0 <= i && i <= 7)
      ProtocolGuid.Data1 = ProtocolGuid.Data1*16 + Temp[i];
    else if(8 <= i && i <= 11)
      ProtocolGuid.Data2 = ProtocolGuid.Data2*16 + Temp[i];
    else if(12 <= i && i <= 15)
      ProtocolGuid.Data3 = ProtocolGuid.Data3*16 + Temp[i];
    else if(16 <= i && i <= 31)
    {
      if(16 <= i && i <= 17)
        ProtocolGuid.Data4[0] = ProtocolGuid.Data4[0]*16 + Temp[i];
      else if(18 <= i && i <= 19)
        ProtocolGuid.Data4[1] = ProtocolGuid.Data4[1]*16 + Temp[i];
      else if(20 <= i && i <= 21)
        ProtocolGuid.Data4[2] = ProtocolGuid.Data4[2]*16 + Temp[i];
      else if(22 <= i && i <= 23)
        ProtocolGuid.Data4[3] = ProtocolGuid.Data4[3]*16 + Temp[i];
      else if(24 <= i && i <= 25)
        ProtocolGuid.Data4[4] = ProtocolGuid.Data4[4]*16 + Temp[i];
      else if(26 <= i && i <= 27)
        ProtocolGuid.Data4[5] = ProtocolGuid.Data4[5]*16 + Temp[i];
      else if(28 <= i && i<= 29)
        ProtocolGuid.Data4[6] = ProtocolGuid.Data4[6]*16 + Temp[i];
      else if(30 <= i && i <= 31)
        ProtocolGuid.Data4[7] = ProtocolGuid.Data4[7]*16 + Temp[i];      
    }
    
    i++; 
  }
  
  return ProtocolGuid;
}


//HandleNumberHandle
EFI_STATUS HandleNumberHandle(VOID)
{
  EFI_STATUS   Status;
  //LocateHandleBuffer
  UINTN        HandleAmount;
  EFI_HANDLE   *HandleArray;
  UINTN        HandleNumber;
  
  //ProtocolsPerHandle
  EFI_GUID     **ProtocolArray;
  UINTN        ProtocolAmount;
  UINT8        j = 0;
  UINT8        k = 0;
  
  Status = gBS -> LocateHandleBuffer(AllHandles, NULL, NULL, &HandleAmount, &HandleArray);
  Print(L"\nPlease Select the handle number you want to dump(0~%d): ", HandleAmount-1);
  HandleNumber = GetHandleNumber();
  Print(L"\nHandle[%d] (address: 0x%p)\n", HandleNumber, HandleArray[HandleNumber]);
  Status = gBS -> ProtocolsPerHandle(HandleArray[HandleNumber], &ProtocolArray, &ProtocolAmount);
  
  for(j = 0; j < ProtocolAmount; j++)
  {
    Print(L"Protocol[%d]: %08X-%04X-", j, ProtocolArray[j] -> Data1, ProtocolArray[j] -> Data2);
    Print(L"%04X-", ProtocolArray[j] -> Data3);
    for(k = 0; k < 8; k++)
      Print(L"%02X",  ProtocolArray[j] -> Data4[k]);
    Print(L"\n");
  }
  
  return Status;  
}
//GetHandleNumber
UINTN GetHandleNumber(VOID)
{
  UINTN   HandleNumber = 0;
  CHAR16  Temp[8];
  UINT8    i = 0;
  
  while((Temp[i] = GetUnicodeChar()) != 13)
  {
    Print(L"%c", Temp[i]);
    HandleNumber = HandleNumber*10 + Temp[i] - '0';
    i++;
  }
  return HandleNumber;
}










