#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
//UefiMain
VOID MemoryAllocateServiceMenu(VOID);
CHAR16 MemoryAllocateServiceFunction(VOID);
//GetUnicodeChar
CHAR16 GetUnicodeChar(VOID);
//AllocatePool
EFI_STATUS AllocatePoolFunction(VOID);
VOID MemoryTypeMenu(VOID);
CHAR16 GetMemoryType(VOID);
UINTN GetAllocatePoolSize(VOID);
//AllocatePages
EFI_STATUS AllocatePagesFunction(VOID);
CHAR16 GetPagesType(VOID);
VOID PagesTypeMenu(VOID);
UINTN GetPages(VOID);
//GetMemoryMapFunction
EFI_STATUS GetMemoryMapFunction (VOID);


EFI_STATUS EFIAPI UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS  Status = 0;
  CHAR16      AllocateServiceChoose;
  
  MemoryAllocateServiceMenu();
  AllocateServiceChoose = MemoryAllocateServiceFunction();
  while(AllocateServiceChoose != 'q')
  {
    if(AllocateServiceChoose == '1')
      Status = AllocatePoolFunction();
    else if(AllocateServiceChoose == '2')
      Status = AllocatePagesFunction();
    else
      Print(L"\n\nChoose error!!! Please choose again.");
    MemoryAllocateServiceMenu();
    AllocateServiceChoose = MemoryAllocateServiceFunction();
  }
  
  Status = GetMemoryMapFunction();
  
  return EFI_SUCCESS;
}
//MemoryAllocateServiceMenu
VOID MemoryAllocateServiceMenu(VOID)
{
  Print(L"\n<<memory allocate service menu>>\n");
  Print(L"1. AllocatePool()\n");
  Print(L"2. AllocatePages()\n");
  Print(L"Please choose a allocate service:");
  Print(L"('q' to quit and check memory map): ");
}
//MemoryAllocateServiceFunction
CHAR16 MemoryAllocateServiceFunction(VOID)
{
  CHAR16   MemoryAllocateServiceChoose[3];
  UINT8    i = 0;
  while((MemoryAllocateServiceChoose[i] = GetUnicodeChar()) != 13)
  {
    Print(L"%c", MemoryAllocateServiceChoose[i]);
    i++;
  }
  return MemoryAllocateServiceChoose[0];
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


//AllocatePoolFunction
EFI_STATUS AllocatePoolFunction(VOID)
{
  EFI_STATUS        Status = 0;
  VOID              *PoolAddress;
  CHAR16            MemoryTypeChoose;
  UINTN             AllocatePoolSize;
  
  MemoryTypeMenu();  
  MemoryTypeChoose = GetMemoryType();
  Print(L"\nHow many bytes do you want to allocate: ");
  AllocatePoolSize = GetAllocatePoolSize();
  Status = gBS -> AllocatePool(MemoryTypeChoose, AllocatePoolSize, &PoolAddress);
  Print(L"\nAllocate %d Bytes at 0x%p\n", AllocatePoolSize, PoolAddress);
  Print(L"***Free Memory Pool***\n");
  Status = gBS -> FreePool(PoolAddress);
  Print(L"Free %d Bytes at 0x%p\n",AllocatePoolSize, PoolAddress);

  return Status; 
}
//MemoryTypeMenu
VOID MemoryTypeMenu(VOID)
{
  Print(L"\n<<Memory type menu>>\n");
  Print(L"0. EfiReservedMemoryType\n");
  Print(L"1. EfiLoaderCode\n");
  Print(L"2. EfiLoaderData\n");
  Print(L"3. EfiBootServicesCode\n");
  Print(L"4. EfiBootServicesData\n");
  Print(L"5. EfiRuntimeServicesCode\n");
  Print(L"6. EfiRuntimeServicesData\n");
  Print(L"Please choose a memory type:");
}
//GetMemoryType
CHAR16 GetMemoryType(VOID)
{
  CHAR16   MemoryTypeChoose[2];
  UINT8    i = 0; 
  
  while((MemoryTypeChoose[i] = GetUnicodeChar()) != 13)
  {
    Print(L"%c", MemoryTypeChoose[i]);
    i++;
  }
  
  MemoryTypeChoose[0] = MemoryTypeChoose[0] - '0'; 
  return MemoryTypeChoose[0];   
}
//GetAllocatePoolSize
UINTN GetAllocatePoolSize(VOID)
{
  UINTN    AllocatePoolSize = 0;
  CHAR16   Temp[8];
  UINT8    i = 0;
  
  while((Temp[i] = GetUnicodeChar()) != 13)
  { 
    Print(L"%c", Temp[i]);
    AllocatePoolSize = AllocatePoolSize*10 + Temp[i] - '0';
    i++;
  }
  
  return AllocatePoolSize;
}


/*AllocatePagesFunction*/
EFI_STATUS AllocatePagesFunction(VOID)
{
  EFI_STATUS   Status;
  CHAR16       PagesTypeChoose;
  CHAR16       MemoryTypeChoose;
  UINTN        Pages;
  EFI_PHYSICAL_ADDRESS   PagesAddress;

  PagesTypeMenu();
  PagesTypeChoose = GetPagesType();
  MemoryTypeMenu();
  MemoryTypeChoose = GetMemoryType();
  Print(L"\nHow many pages do yo want to allocate: ");
  Pages = GetPages();
  Status = gBS -> AllocatePages(PagesTypeChoose, MemoryTypeChoose, Pages, &PagesAddress);
  Print(L"\nAllocate %d Pages(%d Bytes) at 0x%p \n", Pages, Pages *4096, PagesAddress);
  Print(L"***Free Pages***\n");
  Status = gBS -> FreePages(PagesAddress, Pages);
  Print(L"Free %d Pages(%d Bytes) at 0x%p\n", Pages, Pages*4096, PagesAddress);
  
  return Status;
}
//PagesTypeMenu
VOID PagesTypeMenu(VOID)
{
  Print(L"\n<<AllocatePages type menu>>\n");
  Print(L"1. AllocateAnyPages\n");
  Print(L"2. AllocateMaxaddress\n");
  Print(L"3. AllocateAddress\n");
  Print(L"4. MaxAllocateType\n");
  Print(L"Please choose a AllocatePages type: ");
}
//GetPagesType
CHAR16 GetPagesType(VOID)
{
  CHAR16   PagesTypeChoose[3];
  UINT8    i = 0;
  
  while((PagesTypeChoose[i] = GetUnicodeChar()) != 13)
  {
    Print(L"%c", PagesTypeChoose[i]);
    i++;
  }
  
  PagesTypeChoose[0] = PagesTypeChoose[0] - '0';
  return PagesTypeChoose[0];
}
//GetPages
UINTN GetPages(VOID)
{
  UINTN    Pages = 0;
  CHAR16   Temp[8];
  UINT8    i = 0;
  
  while((Temp[i] = GetUnicodeChar()) != 13)
  { 
    Print(L"%c", Temp[i]);
    Pages = Pages*10 + Temp[i] - '0';
    i++;
  }

  return Pages;
}


//GetMemoryMap
EFI_STATUS    GetMemoryMapFunction(VOID)
{
  EFI_STATUS                  Status = 0;
  UINTN                       MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR       *MemoryMap = 0;
  EFI_MEMORY_DESCRIPTOR       *MemoryMapTemp = 0;
  UINTN                       MapKey = 0;
  UINTN                       DescriptorSize = 0;
  UINT32                      DescriptorVersion = 0;
  UINTN                       i = 0;
//  UINT64   MemoryType[20];
  CHAR16   *MemoryTypeMenu[20] = {
          L"EfiReservedMemoryType",
          L"EfiLoaderCode",
          L"EfiLoaderData",
          L"EfiBootServicesCode",
          L"EfiBootServicesData",
          L"EfiRuntimeServicesCode",
          L"EfiRuntimeServicesData",
          L"EfiConventionalMemory",
          L"EfiUnusableMemory",
          L"EfiACPIReclaimMemory",
          L"EfiACPIMemoryNVS",
          L"EfiMemoryMappedIO",
          L"EfiMemoryMappedIOPortSpace",
          L"EfiPalCode",
          L"EfiPersistentMemory",
          L"EfiMaxMemoryType"
          };
          
/*获得系统内存映射*/  
  Status = gBS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  Status = gBS -> AllocatePool(EfiBootServicesData, MemoryMapSize, (void**)&MemoryMap);
  Status = gBS -> GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  
//  for (i = 0; i < 20; i++)
//    MemoryType[i] = 0;
  
  /*打印系统内所有内存映射的信息*/  
  Print(L"\nMemType                       Pages      PhyAddr   VirAddr\n");
  for( i = 0; i < MemoryMapSize / DescriptorSize; i++)
  {
    MemoryMapTemp = (EFI_MEMORY_DESCRIPTOR*) ( ((CHAR8*)MemoryMap) + i * DescriptorSize);
    Print(L"%02d-%-26s", MemoryMapTemp -> Type, MemoryTypeMenu[MemoryMapTemp -> Type]);
    Print(L"%6d: ", MemoryMapTemp -> NumberOfPages);
    Print(L"    0x%p--", MemoryMapTemp -> PhysicalStart);
    Print(L"0x%p\n", MemoryMapTemp -> VirtualStart);
//    MemoryType[MemoryMapTemp -> Type] = MemoryType[MemoryMapTemp -> Type] + MemoryMapTemp -> NumberOfPages;
  }
/*  for (i = 0; i < EfiMaxMemoryType; i++)
  {
    if (MemoryType[MemoryMapTemp -> Type] != 0)
    {
      Print(L"%02d%s: ", MemoryMapTemp -> Type, MemoryTypeMenu[MemoryMapTemp -> Type]);
      Print(L"%6d Pages (%d)\n", MemoryType[MemoryMapTemp -> Type], MemoryType[MemoryMapTemp -> Type]*4096);
    }
  }*/
  Status = gBS -> FreePool(MemoryMap);
  return Status;
}



