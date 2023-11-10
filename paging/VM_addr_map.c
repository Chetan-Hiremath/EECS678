#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAXSTR 1000

int main(int argc, char *argv[])
{
  char line[MAXSTR];
  int *page_table, *mem_map, *frame_map;
  unsigned int log_size, phy_size, page_size, d;
  unsigned int num_pages, num_frames;
  unsigned int offset, logical_addr, physical_addr, page_num, frame_num;

  /* Get the memory characteristics from the input file */
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Logical address space size: %d^%d", &d, &log_size)) != 2){
    fprintf(stderr, "Unexpected line 1. Abort.\n");
    exit(-1);
  }
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Physical address space size: %d^%d", &d, &phy_size)) != 2){
    fprintf(stderr, "Unexpected line 2. Abort.\n");
    exit(-1);
  }
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Page size: %d^%d", &d, &page_size)) != 2){
    fprintf(stderr, "Unexpected line 3. Abort.\n");
    exit(-1);
  }

  /* Allocate arrays to hold the page table and memory frames map */
  d = log_size - page_size;
  num_pages = (unsigned int) pow(2, d);
  page_table = (int *)calloc(num_pages , sizeof(int));
  
  d = log_size - page_size;
  num_frames = (unsigned int) pow(2, d);
  mem_map = (int *)calloc(num_pages , sizeof(int));
  frame_map = (int *)calloc(num_frames , sizeof(int));
  
  
  unsigned int offset_mask = 0xffffffff >> (unsigned)log2(num_pages);
  /* Initialize page table to indicate that no pages are currently mapped to
     physical memory */
  /*for (int i = 0; i < num_pages; i++)
  {
    page_table[i] = NULL;
  }
  */
  fprintf(stdout, "Number of Pages: %d, Number of Frames: %d\n", num_pages, num_frames);
  

  /* Initialize memory map table to indicate no valid frames */  

  /* Read each accessed address from input file. Map the logical address to
     corresponding physical address */
  fgets(line, MAXSTR, stdin);
  while(!(feof(stdin))){
    sscanf(line, "0x%x", &logical_addr);
    fprintf(stdout, "Logical address: 0x%x\n", logical_addr);
    
	/* Calculate page number and offset from the logical address */
	page_num = logical_addr >> page_size;
    fprintf(stdout, "Page Number: 0x%x\n", page_num);
    if (!mem_map[page_num]) {
        fprintf(stdout, "Page Fault!\n");
        for (int i = 0; i < num_frames; i++)
        {
          if (!frame_map[i])
          {
            page_table[page_num] = i;
            mem_map[page_num] = 1;
            frame_map[i] = 1;
          }
        }
    }
    fprintf(stdout, "Frame Number: 0x%x\n", frame_num);

    /* Form corresponding physical address */
    physical_addr = (page_table[page_num] << page_size) | (logical_addr & offset_mask);  
    fprintf(stdout, "Physical address: 0x%x\n", physical_addr);
    
    /* Read next line */
    fgets(line, MAXSTR, stdin);    
  }

  return 0;
}
