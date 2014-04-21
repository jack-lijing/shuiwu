#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include <water.h>

void *c=NULL;
	Recv 	R 	= {{""}, 0, 0, 0};
	Send	S	= {{""}, HEADLEN_26 , SUCCESS, "", 0};
	//struct 	water 	winfo	= { "", 0.0, 0, Calloc(MONTHNUM,sizeof(struct bill) )} ;			//水费账户/
static	struct 	water 	winfo;
	//struct 	bank 	binfo	= { "", 0.0, Calloc(1,sizeof(struct pay))};					//银行账户 
static	struct 	bank 	binfo;
static	struct 	user 	person;
int init_suite1(void)
{


	/*winfo	= { "", 0.0, 0, Calloc(MONTHNUM,30 )} ;			//水费账户/
	binfo	= { "", 0.0, 30}//银行账户 
	person 	= { "", "", &winfo, &binfo };							//用户信息
	memset( R.buffer, 0 , sizeof(R.buffer));
	memset( S.buffer, 0 , sizeof(S.buffer));
	memset( S.file, 0 , sizeof(S.file));
*/
}



int clean_suite1(void)
{
}

void test7000(void)
{
	do7000(&S, &person, &R ,c);
	CU_ASSERT(0 == 0);

}

int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test of fprintf()", test7000))) 
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
