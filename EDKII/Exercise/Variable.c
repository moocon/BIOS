#include <Uefi.h> 
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>//Provide "gST gBS"
#include <Library/UefiRuntimeServicesTableLib.h>//Provide "gRT"
#include <Library/MemoryAllocationLib.h>//Provide "AllocateZeroPool()"
#include <Library/BaseMemoryLib.h>//Provide "CompareGuid()"

//UefiMain
VOID FunctionSelectMenu(VOID);
CHAR16 GetFunctionSelectResult(VOID);
//GetUnicodeChar
CHAR16 GetUnicodeChar(VOID);

//ListVariable
EFI_STATUS ListVariable(VOID);
EFI_STATUS GetVariableInformation(CHAR16 *VariableName, EFI_GUID *VariableGuid);

//NameSearch
EFI_STATUS NameSearch(VOID);

//GuidSearch
EFI_STATUS GuidSearch(VOID);
EFI_GUID GetSearchGuid(VOID);

//CreateVariable
EFI_STATUS CreateVariable(VOID);

//DeleteVariable
EFI_STATUS DeleteVariable(VOID);

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS   Status;
  CHAR16       FunctionSelectResult;
  
  FunctionSelectMenu();
  while((FunctionSelectResult = GetFunctionSelectResult()) != 6)
  {
    if(FunctionSelectResult == 1)
      Status = ListVariable();
    else if(FunctionSelectResult == 2)
      Status = NameSearch();
    else if(FunctionSelectResult == 3)
      Status = GuidSearch();
    else if(FunctionSelectResult == 4)
      Status = CreateVariable();
    else if(FunctionSelectResult == 5)
      Status = DeleteVariable();   
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
  Print(L"1. List all variables\n");
  Print(L"2. Search Variable by variable name\n");
  Print(L"3. Search Variable by variable GUID\n");
  Print(L"4. Creat a new Varible with specific attributes and allow input data for it\n");
  Print(L"5. Delete a existing Varibles from the system.\n");
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


//ListVariable
EFI_STATUS ListVariable(VOID)
{ 
  UINTN      Status = 0;
  UINTN      VariableNameSize = 60;
  CHAR16     *VariableName = NULL;
  EFI_GUID   VariableGuid;
  UINT8      k = 0;
  
  VariableName = AllocateZeroPool(VariableNameSize);
  while(1)
  {
    VariableNameSize = 60;
    Status = gRT -> GetNextVariableName(&VariableNameSize, VariableName, &VariableGuid);
    if(Status == EFI_NOT_FOUND)
      break; 
    Print(L"\n%-30s", VariableName);
    Print(L"   %08X-%04X-%04X-", VariableGuid.Data1, VariableGuid.Data2, VariableGuid.Data3);
    for(k = 0; k < 8; k++)
      Print(L"%02X",  VariableGuid.Data4[k]);
/*invoke GetVariableInformation*/
    Status = GetVariableInformation(VariableName, &VariableGuid);
  }

  
  Print(L"\n***VariableName***               ***VariableGuid***\n");
  return Status;
}
//GetVariableInformation
EFI_STATUS GetVariableInformation(CHAR16 *VariableName, EFI_GUID *VariableGuid)
{
  EFI_STATUS   Status;
  UINT32       VariableAttribute;
  UINTN        VariableDataSize = 1024;
  CHAR16       *VariableData;
  
  VariableData = AllocateZeroPool(VariableDataSize);
  Status = gRT -> GetVariable(VariableName, VariableGuid, &VariableAttribute, &VariableDataSize, VariableData);
  Print(L"\n  Attribute: 0x%X\n", VariableAttribute);
  Print(L"  Data: %s", VariableData);
  
  return Status;
}


//NameSearch
EFI_STATUS NameSearch(VOID)
{
  EFI_STATUS      Status = 0;

  CHAR16     P1[] = L"12";
  CHAR16     P2[] = L"12";
  CHAR16     SearchName[30];
  CHAR16     Temp;
  UINT8      i = 0;

  UINTN      VariableNameSize = 60;
  CHAR16     *VariableName;
  EFI_GUID   VariableGuid;
  UINT8      k = 0;
//  UINT8      SearchHit = 0;
  
/*Get Search Name*/  
  Print(L"\n>>SearchName: ");
  while((Temp = GetUnicodeChar()) != 13)
  {
    SearchName[i] = Temp;
    Print(L"%c", SearchName[i]);
    i++;
  }
  Print(L"\nSearchName: %s\n", SearchName);
///////////////////////////////////////////////////////////////////////  
  Print(L"\nP1: %s\n", P1);//
  Print(L"P2: %s\n", P2);//
  if(StrCmp(P2, P1) == 0) // This is a question!!! need to understand
    Print(L"\nP1P2StrCmp is useful\n");//
///////////////////////////////////////////////////////////////////////    
  VariableName = AllocateZeroPool(VariableNameSize);
  Print(L"***VariableName***               ***VariableGuid***\n");
  while(1)
  {
    VariableNameSize = 60;
    Status = gRT -> GetNextVariableName(&VariableNameSize, VariableName, &VariableGuid);
    if (StrCmp(SearchName, VariableName) == 0)
    {
      Print(L"%-30s", VariableName);
      Print(L"   %08X-%04X-%04X-", VariableGuid.Data1, VariableGuid.Data2, VariableGuid.Data3);
      for(k = 0; k < 8; k++)
        Print(L"%02X",  VariableGuid.Data4[k]);
      Status = GetVariableInformation(VariableName, &VariableGuid);
      break;
    }
    if(Status == EFI_NOT_FOUND)
    {
//      if(SearchHit == 0)
       Print(L"\nError!!! Not find this variable in system");
      break;
    }
  }
  return Status; 
}


//GuidSearch
EFI_STATUS GuidSearch(VOID)
{
  EFI_STATUS   Status;
  EFI_GUID     SearchGuid;
  
  UINTN      VariableNameSize = 60;
  CHAR16     *VariableName = NULL;
  EFI_GUID   VariableGuid;
  UINT8      k = 0;
  UINT8      SearchHit = 0;
  
  Print(L"\n>>SearchGuid: ");
  SearchGuid = GetSearchGuid();

  VariableName = AllocateZeroPool(VariableNameSize);
  Print(L"\n***VariableName***               ***VariableGuid***");
  while(1)
  {
    VariableNameSize = 60;
    Status = gRT -> GetNextVariableName(&VariableNameSize, VariableName, &VariableGuid);
    if (CompareGuid(&SearchGuid, &VariableGuid))
    {
      Print(L"\n%-30s", VariableName);
      Print(L"   %08X-%04X-%04X-", VariableGuid.Data1, VariableGuid.Data2, VariableGuid.Data3);
      for(k = 0; k < 8; k++)
        Print(L"%02X",  VariableGuid.Data4[k]);
      Status = GetVariableInformation(VariableName, &VariableGuid);
      SearchHit++;
    }
    if(Status == EFI_NOT_FOUND)
    {
      if(SearchHit == 0)
        Print(L"\nError!!! Not find this variable in system");
      break;
    }
  }  

  return Status;
}
//GetSearchGuid
EFI_GUID GetSearchGuid(VOID)
{
  UINT8    Temp[33];
  EFI_GUID  VariableGuid;  
  UINT8    i = 0;
  UINT8    j = 0;
  
  VariableGuid.Data1 = 0;
  VariableGuid.Data2 = 0;
  VariableGuid.Data3 = 0;
  for(j = 0; j < 8; j++)
    VariableGuid.Data4[j] = 0;
  
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
      VariableGuid.Data1 = VariableGuid.Data1*16 + Temp[i];
    else if(8 <= i && i <= 11)
      VariableGuid.Data2 = VariableGuid.Data2*16 + Temp[i];
    else if(12 <= i && i <= 15)
      VariableGuid.Data3 = VariableGuid.Data3*16 + Temp[i];
    else if(16 <= i && i <= 31)
    {
      if(16 <= i && i <= 17)
        VariableGuid.Data4[0] = VariableGuid.Data4[0]*16 + Temp[i];
      else if(18 <= i && i <= 19)
        VariableGuid.Data4[1] = VariableGuid.Data4[1]*16 + Temp[i];
      else if(20 <= i && i <= 21)
        VariableGuid.Data4[2] = VariableGuid.Data4[2]*16 + Temp[i];
      else if(22 <= i && i <= 23)
        VariableGuid.Data4[3] = VariableGuid.Data4[3]*16 + Temp[i];
      else if(24 <= i && i <= 25)
        VariableGuid.Data4[4] = VariableGuid.Data4[4]*16 + Temp[i];
      else if(26 <= i && i <= 27)
        VariableGuid.Data4[5] = VariableGuid.Data4[5]*16 + Temp[i];
      else if(28 <= i && i<= 29)
        VariableGuid.Data4[6] = VariableGuid.Data4[6]*16 + Temp[i];
      else if(30 <= i && i <= 31)
        VariableGuid.Data4[7] = VariableGuid.Data4[7]*16 + Temp[i];      
    }
    
    i++; 
  }
  
  return VariableGuid;
}


//CreateVariable
EFI_STATUS CreateVariable(VOID)
{
  EFI_STATUS   Status;
  CHAR16       VariableName[60];
  CHAR16       Temp;
  UINT8        i = 0;
  UINT8        k = 0;
  EFI_GUID     VariableGuid = {0x47C7B227, 0xC42A, 0x11D2, {0x8E, 0x57, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}};
  UINT32       VariableAttribute = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_APPEND_WRITE;
  UINTN        VariableDataSize = 60;
  CHAR16       *VariableData = L"Jason"; 
  
  Print(L"\n>>VariableName: ");
  while ((Temp = GetUnicodeChar()) != 13)
  {
    VariableName[i] = Temp;
    Print(L"%c", VariableName[i]);
    i++;
  }
  Status = gRT -> SetVariable(VariableName, &VariableGuid, VariableAttribute, VariableDataSize, VariableData);
  Print(L"\n***CreateVariableStatus: 0x%x***\n", Status);
  if(!EFI_ERROR(Status)){
    Print(L"Create variable successfully:\n"); 
    Print(L"Name: %s", VariableName);
    Print(L"   Guid: %08X-%04X-%04X-", VariableGuid.Data1, VariableGuid.Data2, VariableGuid.Data3);
    for(k = 0; k < 8; k++)
      Print(L"%02X",  VariableGuid.Data4[k]);
    Print(L"\n\n");
  }  
  return Status;
}


//DeleteVariable
EFI_STATUS DeleteVariable(VOID)
{
  EFI_STATUS   Status;
  CHAR16     P1[] = L"12";
  CHAR16     P2[] = L"12";
  CHAR16     SearchName[30];
  CHAR16     Temp;
  UINT8      i = 0;

  UINTN      VariableNameSize = 60;
  CHAR16     *VariableName;
  EFI_GUID   VariableGuid;
  
  UINT8      k = 0;
//  UINT8      SearchHit = 0;
  
/*Get Search Name*/  
  Print(L"\n>>SearchName: ");
  while((Temp = GetUnicodeChar()) != 13)
  {
    SearchName[i] = Temp;
    Print(L"%c", SearchName[i]);
    i++;
  }
  Print(L"\nSearchName: %s\n", SearchName);
///////////////////////////////////////////////////////////////////////  
  Print(L"\nP1: %s\n", P1);//
  Print(L"P2: %s\n", P2);//
  if(StrCmp(P2, P1) == 0) // This is a question!!! need to understand
    Print(L"\nP1P2StrCmp is useful\n");//
///////////////////////////////////////////////////////////////////////    
  VariableName = AllocateZeroPool(VariableNameSize);
  while(1)
  {
    VariableNameSize = 60;
    Status = gRT -> GetNextVariableName(&VariableNameSize, VariableName, &VariableGuid);
    if (StrCmp(SearchName, VariableName) == 0)
    {
      Status = gRT -> SetVariable(VariableName, &VariableGuid, 0, 0, NULL);
      Print(L"DeleteVariableStatus: 0x%x\n", Status);
      if (!EFI_ERROR(Status))
      {
        Print(L"Delete Variable successfully:\n");
        Print(L"Name: %s", VariableName);
        Print(L"   Guid: %08X-%04X-%04X-", VariableGuid.Data1, VariableGuid.Data2, VariableGuid.Data3);
        for(k = 0; k < 8; k++)
          Print(L"%02X",  VariableGuid.Data4[k]);
        Print(L"\n\n");
      } 
      break;
    }
    if(Status == EFI_NOT_FOUND)
    {
//      if(SearchHit == 0)
       Print(L"\nError!!! Not find this variable in system");
      break;
    }
  }  
  
  return Status;
}


















