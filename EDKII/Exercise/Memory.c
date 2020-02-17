#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

//UefiMain
VOID MemoryAllocateServiceMenu(VOID);
CHAR16 MemoryAllocateServiceFunction(VOID);

//AllocatePages
EFI_STATUS AllocatePagesFunction (VOID);
CHAR16 PagesTypeChooseFunction(VOID);
VOID PagesTypeMenu(VOID);
UINTN PagesFunction(VOID);

//AllocatePool
EFI_STATUS AllocatePoolFunction(VOID);
VOID MemoryTypeMenu(VOID);
CHAR16 MemoryTypeChooseFunction(VOID);
UINTN AllocatePoolSizeFunction(VOID);

//GetUnicodeChar
CHAR16 GetUnicodeChar(VOID);

//GetMemoryMapInformation
EFI_STATUS GetMemoryMapFunction (VOID);


EFI_STATUS
EFIAPI
UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE  *SystemTable)
{
  EFI_STATUS  Status = 0;
  CHAR16      AllocateServiceChoose;
  
  MemoryAllocateServiceMenu();
  AllocateServiceChoose = MemoryAllocateServiceFunction();
  while(AllocateServiceChoose != 'q')
  {
    if(AllocateServiceChoose == '1')
      AllocatePagesFunction();
    else if(AllocateServiceChoose == '2')
      AllocatePoolFunction();
    else
      Print(L"Choose error, Please choose again\n");
    MemoryAllocateServiceMenu();
    AllocateServiceChoose = MemoryAllocateServiceFunction();
  }
  
  Status = GetMemoryMapFunction();
  return Status;
}
//MemoryAllocateServiceMenu
VOID MemoryAllocateServiceMenu(VOID)
{
  Print(L"\n<<Choose a memory allocate service>>\n");
  Print(L"1. AllocatePages()\n");
  Print(L"2. AllocatePool()\n");
  Print(L"Please choose a allocation service:");
  Print(L"('q' to quit): ");
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


/*AllocatePagesFunction*/
EFI_STATUS AllocatePagesFunction(VOID)
{
  EFI_STATUS   Status;
  CHAR16       PagesTypeChoose;
  CHAR16       MemoryTypeChoose;
  UINTN        Pages;
  EFI_PHYSICAL_ADDRESS   PagesAddress;

  PagesTypeMenu();
  PagesTypeChoose = PagesTypeChooseFunction();
  MemoryTypeMenu();
  MemoryTypeChoose = MemoryTypeChooseFunction();
  Print(L"\nHow many pages do yo want to allocate: ");
  Pages = PagesFunction();
  Status = gBS -> AllocatePages(PagesTypeChoose, MemoryTypeChoose, Pages, &PagesAddress);
  Print(L"\nAllocate %d Pages(%d Bytes) at 0x%p \n", Pages, Pages *4096, PagesAddress);
  Print(L"\n***Free Pages***\n");
  Status = gBS -> FreePages(PagesAddress, Pages);
  Print(L"Free %d Pages(%d Bytes) at 0x%p\n", Pages, Pages*4096, PagesAddress);
  
  return Status;
}
//PagesTypeMenu
VOID PagesTypeMenu(VOID)
{
  Print(L"\n<<Choose a allocation type>>\n");
  Print(L"1. AllocateAnyPages\n");
  Print(L"2. AllocateMaxaddress\n");
  Print(L"3. AllocateAddress\n");
  Print(L"4. MaxAllocateType\n");
  Print(L"Please Choose a allocation type: ");
}
//PagesTypeChooseFunction
CHAR16 PagesTypeChooseFunction(VOID)
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
//PagesFunction
UINTN PagesFunction(VOID)
{
  UINTN    Pages = 0;
  CHAR16   Temp[8];
  UINT8    i = 0;
  UINT8    i_temp = 0;
  UINT8    j = 0;
  
  while((Temp[j] = GetUnicodeChar()) != 13)
  { 
    Print(L"%c", Temp[j]);
    Temp[j] = Temp[j] - '0';
    j++;
  }
  
  for(i = 0; i < j; i++)
  {
    for (i_temp = i; i_temp < (j - 1); i_temp++)
      Temp[i] = Temp[i] * 10;
    Pages =  Pages + Temp[i];   
  }
  return Pages;
}



/*AllocatePoolFunction*/
EFI_STATUS AllocatePoolFunction(VOID)
{
  EFI_STATUS        Status = 0;
  VOID              *PoolAddress;
  
  CHAR16            MemoryTypeChoose;
  UINTN             AllocatePoolSize;
  
  MemoryTypeMenu();  
  MemoryTypeChoose = MemoryTypeChooseFunction();
  Print(L"\nHow many bytes do you want to allocate: ");
  AllocatePoolSize = AllocatePoolSizeFunction();
  Status = gBS -> AllocatePool(MemoryTypeChoose, AllocatePoolSize, &PoolAddress);
  Print(L"\nAllocate %d Bytes at 0x%p\n", AllocatePoolSize, PoolAddress);
  Print(L"\n***Free Memory Pool***\n");
  Status = gBS -> FreePool(PoolAddress);
  Print(L"Free %d Bytes at 0x%p\n",AllocatePoolSize, PoolAddress);
   
/*  switch(MemoryTypeChoose)
  {
    case '0': 
      Status = gBS -> AllocatePool(EfiReservedMemoryType, AllocatePoolSize, &MemoryMapBufferHeadAddress);
      Print(L"\nAllocatepool:[%d]Bytes at (%08x)\n",(UINTN)AllocatePoolSize, MemoryMapBufferHeadAddress);
      break;
    case '1': 
      Status = gBS -> AllocatePool(EfiLoaderCode, AllocatePoolSize, &MemoryMapBufferHeadAddress);
      Print(L"\nAllocatepool:[%d]Bytes at (%08x)\n",(UINTN)AllocatePoolSize, MemoryMapBufferHeadAddress);
      break;
    case '2': 
      Status = gBS -> AllocatePool(EfiLoaderData, AllocatePoolSize, &MemoryMapBufferHeadAddress);
      Print(L"\nAllocatepool:[%d]Bytes at (%08x)\n",(UINTN)AllocatePoolSize, MemoryMapBufferHeadAddress);
      break;
    case '3': 
      Status = gBS -> AllocatePool(EfiBootServicesCode, AllocatePoolSize, &MemoryMapBufferHeadAddress);
      Print(L"\nAllocatepool:[%d]Bytes at (%08x)\n",(UINTN)AllocatePoolSize, MemoryMapBufferHeadAddress);
      break;
    case '4':
      Status = gBS -> AllocatePool(EfiBootServicesCode, AllocatePoolSize, &MemoryMapBufferHeadAddress);
      Print(L"\nAllocatepool:[%d]Bytes at (%08x)\n",(UINTN)AllocatePoolSize, MemoryMapBufferHeadAddress);
      break;
    case '5':
      Status = gBS -> AllocatePool(EfiBootServicesCode, AllocatePoolSize, &MemoryMapBufferHeadAddress);
      Print(L"\nAllocatepool:[%d]Bytes at (%08x)\n",(UINTN)AllocatePoolSize, MemoryMapBufferHeadAddress);
      break;
    case '6':
      Status = gBS -> AllocatePool(EfiBootServicesCode, AllocatePoolSize, &MemoryMapBufferHeadAddress);
      Print(L"\nAllocatepool:[%d]Bytes at (%08x)\n",(UINTN)AllocatePoolSize, MemoryMapBufferHeadAddress);
      break;
    default: 
      Print(L"Choose error, Please choose again\n");
  }*/
    
  return Status; 
}
//MemoryTypeMenu
VOID MemoryTypeMenu(VOID)
{
  Print(L"\n<<Optional memory type>>\n");
  Print(L"0. EfiReservedMemoryType\n");
  Print(L"1. EfiLoaderCode\n");
  Print(L"2. EfiLoaderData\n");
  Print(L"3. EfiBootServicesCode\n");
  Print(L"4. EfiBootServicesData\n");
  Print(L"5. EfiRuntimeServicesCode\n");
  Print(L"6. EfiRuntimeServicesData\n");
  Print(L"Please choose a memory type:");
}
//MemoryTypeChooseFunction
CHAR16 MemoryTypeChooseFunction(VOID)
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
//AllocatePoolSizeFunction
UINTN AllocatePoolSizeFunction(VOID)
{
  UINTN    AllocatePoolSize = 0;
  CHAR16   Temp[8];
  UINT8    i = 0;
  UINT8    i_temp = 0;
  UINT8    j = 0;
  
  while((Temp[j] = GetUnicodeChar()) != 13)
  { 
    Print(L"%c", Temp[j]);
    Temp[j] = Temp[j] - '0';
    j++;
  }
  for(i = 0; i < j; i++)
  {
    for (i_temp = i; i_temp < (j - 1); i_temp++)
      Temp[i] = Temp[i] * 10;
    AllocatePoolSize =  AllocatePoolSize + Temp[i];   
  }
  return AllocatePoolSize;
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






//GetMemoryMapFunction
EFI_STATUS    GetMemoryMapFunction(VOID)
{
  EFI_STATUS                  Status = 0;
  UINTN                       MemoryMapBufferSize = 0;
  EFI_MEMORY_DESCRIPTOR       *MemoryMapBufferHeadAddress = 0;
  EFI_MEMORY_DESCRIPTOR       *MemoryMapBufferHeadAddressTemp = 0;
  UINTN                       MemoryMapKey = 0;
  UINTN                       EfiMemoryDescriptorSize =0;
  UINT32                      EfiDescriptorVersion = 0; 
  UINTN                       i;

/*查询目前系统内存映射的信息需要独大的缓冲区来存储
  MemoryMapSize：IN：？ OUT：先准备足够的缓冲空间（MemoryMapBufferSize），用来存放系统内所有内存映射的信息。这里做OUT。
  MemoryMapHeadAddress：IN：从allocatepool()那里取得缓冲区的首地址，并将系统内所有内存映射的信息存储于此。OUT：可打印输出缓冲区的内容（即是系统内存映射的信息）
  MemoryMapKey：获取当前内存映射的"key"，是一个整数。这个"Key"是什么？
  EfiMemoryDescriptorSize：获取EFI_MEMORY_DESCRIPTOR结构体的大小（即“每个内存映射”，占缓冲区多大），值会比sizeof(EFI_MEMORY_DESCRIPTOR)大*/
  Print(L"***Get required Buffer size(MemoryMapBufferSize)***\n");
  Status = gBS -> GetMemoryMap(&MemoryMapBufferSize, MemoryMapBufferHeadAddress, &MemoryMapKey, &EfiMemoryDescriptorSize, &EfiDescriptorVersion);
  Print(L"1. MemoryMapBufferSize = %p\n", MemoryMapBufferSize);
  Print(L"MemoryMapBufferHeadAddress =  %p\n", MemoryMapBufferHeadAddress);
  Print(L"MemoryMapKey = %p\n", MemoryMapKey);
  Print(L"EfiMemoryDescriptorSize = %p\n", EfiMemoryDescriptorSize);
  Print(L"sizeof(EFI_MEMORY_DESCRIPTOR) = %p\n", sizeof(EFI_MEMORY_DESCRIPTOR));
  Print(L"GetMemoryMap_ReturnStatus = %p\n", Status);
  Print(L"EFI_BUFFER_TOO_SMALL = %p\n\n", EFI_BUFFER_TOO_SMALL);

/*分配缓冲区给系统内所有的内存映射*/  
  Status = gBS -> AllocatePool(EfiBootServicesData, MemoryMapBufferSize, &MemoryMapBufferHeadAddress);
  
/*读取系统内所有内存映射的信息*/  
  Status = gBS -> GetMemoryMap(&MemoryMapBufferSize, MemoryMapBufferHeadAddress, &MemoryMapKey, &EfiMemoryDescriptorSize, &EfiDescriptorVersion);
  
/*打印系统内所有内存映射的信息*/  
  Print(L"   Type     Pages      PhyAddr   VirAddr\n");
  for( i = 0; i < (MemoryMapBufferSize / EfiMemoryDescriptorSize); i++)
  {
    MemoryMapBufferHeadAddressTemp = MemoryMapBufferHeadAddress;
    MemoryMapBufferHeadAddressTemp = MemoryMapBufferHeadAddressTemp + i * EfiMemoryDescriptorSize;
    Print(L"%02d:", i);
    Print(L"%08p %08p:", MemoryMapBufferHeadAddressTemp -> Type, MemoryMapBufferHeadAddressTemp -> NumberOfPages);
    Print(L"  %08p", MemoryMapBufferHeadAddressTemp -> PhysicalStart);
    Print(L"  %08p\n", MemoryMapBufferHeadAddressTemp -> VirtualStart);
  }
  Status = gBS -> FreePool(MemoryMapBufferHeadAddress);
  return Status;
}



