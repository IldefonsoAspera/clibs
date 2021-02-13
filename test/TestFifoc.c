#include "..\unity\src\unity.h"
#include "..\fifoc.h"
#include "..\fifor.h"
#include "..\logger.h"

#include <stdint.h>
#include <stdio.h>

void test_fifor(void)
{
    FIFOR_DEF(my_fifor, 7);
    uint8_t my_fifor_cmp[5] = {0};
    uint16_t size = 0;
    uint16_t var = 0;

    TEST_ASSERT_TRUE(fifor_is_empty(&my_fifor));
    TEST_ASSERT_FALSE(fifor_read(&my_fifor, &size, &my_fifor_cmp));

    my_fifor_cmp[0] = 0xA5;
    TEST_ASSERT_TRUE(fifor_write(&my_fifor, 1, &my_fifor_cmp[0]));
    TEST_ASSERT_EQUAL_UINT32(0, my_fifor.idx_rd);
    TEST_ASSERT_EQUAL_UINT32(3, my_fifor.idx_wr);
    TEST_ASSERT_TRUE(fifor_read(&my_fifor, &size, &my_fifor_cmp[1]));
    TEST_ASSERT_EQUAL_UINT32(3, my_fifor.idx_rd);
    TEST_ASSERT_EQUAL_UINT32(3, my_fifor.idx_wr);
    TEST_ASSERT_EQUAL_UINT8(0xA5, my_fifor_cmp[1]);
    TEST_ASSERT_TRUE(fifor_is_empty(&my_fifor));
    TEST_ASSERT_FALSE(fifor_read(&my_fifor, &size, &my_fifor_cmp));

    my_fifor_cmp[0] = 0xA5;
    var = 0xDEAD;
    TEST_ASSERT_TRUE(fifor_write(&my_fifor, 1, &my_fifor_cmp[0]));
    TEST_ASSERT_EQUAL_UINT32(3, my_fifor.n_in_use);
    TEST_ASSERT_TRUE(fifor_write(&my_fifor, 2, &var));
    TEST_ASSERT_FALSE(fifor_write(&my_fifor, 1, &my_fifor_cmp[0]));
    TEST_ASSERT_EQUAL_UINT32(7, my_fifor.n_in_use);
    TEST_ASSERT_TRUE(fifor_peek(&my_fifor, &size, &my_fifor_cmp[4]));
    TEST_ASSERT_EQUAL_UINT8(0xA5, my_fifor_cmp[4]);
    TEST_ASSERT_TRUE(fifor_read(&my_fifor, &size, &my_fifor_cmp[0]));
    TEST_ASSERT_EQUAL_UINT8(0xA5, my_fifor_cmp[0]);
    TEST_ASSERT_TRUE(fifor_read(&my_fifor, &size, &var));
    TEST_ASSERT_EQUAL_HEX16(0xDEAD, var);
    TEST_ASSERT_TRUE(fifor_is_empty(&my_fifor));
    TEST_ASSERT_FALSE(fifor_read(&my_fifor, &size, &my_fifor_cmp));

    var = 0xDEAD;
    TEST_ASSERT_TRUE(fifor_write(&my_fifor, 2, &var));
    TEST_ASSERT_FALSE(fifor_write(&my_fifor, 2, &var));
    fifor_flush(&my_fifor);
    TEST_ASSERT_TRUE(fifor_is_empty(&my_fifor));
    TEST_ASSERT_FALSE(fifor_read(&my_fifor, &size, &var));
}




void test_fifoc(void)
{
    FIFOC_DEF(my_fifoc, uint32_t, 5);
    uint32_t my_fifoc_cmp[5] = {0};
    uint32_t my_fifoc_tst = 0;

    TEST_ASSERT_FALSE_MESSAGE(fifoc_get(&my_fifoc, &my_fifoc_tst), "Queue is empty and nothing should be getted");
    TEST_ASSERT_FALSE(fifoc_is_full(&my_fifoc));
    TEST_ASSERT_TRUE(fifoc_is_empty(&my_fifoc));

    my_fifoc_tst = 0x12345678;
    TEST_ASSERT_TRUE_MESSAGE(fifoc_put(&my_fifoc, &my_fifoc_tst), "Queue was empty but item could not be inserted");
    TEST_ASSERT_FALSE(fifoc_is_full(&my_fifoc));
    TEST_ASSERT_FALSE(fifoc_is_empty(&my_fifoc));
    TEST_ASSERT_EQUAL_UINT32(0, my_fifoc.idx_rd);
    TEST_ASSERT_EQUAL_UINT32(1, my_fifoc.idx_wr);
    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_HEX32(0x12345678, my_fifoc_tst);
    TEST_ASSERT_FALSE(fifoc_is_full(&my_fifoc));
    TEST_ASSERT_TRUE(fifoc_is_empty(&my_fifoc));
    TEST_ASSERT_EQUAL_UINT32(1, my_fifoc.idx_rd);
    TEST_ASSERT_EQUAL_UINT32(1, my_fifoc.idx_wr);

    my_fifoc_cmp[0] = 0x12345678;
    my_fifoc_cmp[1] = 0xDEADBEEF;
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[0]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[1]));
    TEST_ASSERT_EQUAL_UINT32(1, my_fifoc.idx_rd);
    TEST_ASSERT_EQUAL_UINT32(3, my_fifoc.idx_wr);
    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_UINT32(2, my_fifoc.idx_rd);
    TEST_ASSERT_EQUAL_UINT32(3, my_fifoc.idx_wr);
    TEST_ASSERT_EQUAL_HEX32(0x12345678, my_fifoc_tst);
    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, my_fifoc_tst);
    TEST_ASSERT_FALSE(fifoc_is_full(&my_fifoc));
    TEST_ASSERT_TRUE(fifoc_is_empty(&my_fifoc));
    
    my_fifoc_cmp[0] = 0x12345678;
    my_fifoc_cmp[1] = 0xDEADBEEF;
    my_fifoc_cmp[2] = 0xC01DC0FE;
    my_fifoc_cmp[3] = 0xABCD1234;
    my_fifoc_cmp[4] = 0xBEEFDEAD;
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[0]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[1]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[2]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[3]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[4]));
    TEST_ASSERT_FALSE(fifoc_put(&my_fifoc, &my_fifoc_cmp[4]));
    TEST_ASSERT_TRUE(fifoc_is_full(&my_fifoc));
    TEST_ASSERT_FALSE(fifoc_is_empty(&my_fifoc));

    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_HEX32(0x12345678, my_fifoc_tst);
    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, my_fifoc_tst);
    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_HEX32(0xC01DC0FE, my_fifoc_tst);
    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_HEX32(0xABCD1234, my_fifoc_tst);
    TEST_ASSERT_TRUE(fifoc_get(&my_fifoc, &my_fifoc_tst));
    TEST_ASSERT_EQUAL_HEX32(0xBEEFDEAD, my_fifoc_tst);
    TEST_ASSERT_FALSE(fifoc_is_full(&my_fifoc));
    TEST_ASSERT_TRUE(fifoc_is_empty(&my_fifoc));
    TEST_ASSERT_FALSE(fifoc_get(&my_fifoc, &my_fifoc_tst));

    my_fifoc_cmp[0] = 0x12345678;
    my_fifoc_cmp[1] = 0xDEADBEEF;
    my_fifoc_cmp[2] = 0xC01DC0FE;
    my_fifoc_cmp[3] = 0xABCD1234;
    my_fifoc_cmp[4] = 0xBEEFDEAD;
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[0]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[1]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[2]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[3]));
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[4]));
    fifoc_flush(&my_fifoc);
    TEST_ASSERT_TRUE(fifoc_put(&my_fifoc, &my_fifoc_cmp[0]));

}



void print_log(char* str, uint16_t length)
{
    printf("%s", str);
}

void test_logger()
{
    log_init(print_log);
    log_debug("Test_string");
    log_debug("Test_string2");   
    TEST_ASSERT_TRUE(log_process());
    TEST_ASSERT_TRUE(log_process());
    TEST_ASSERT_FALSE(log_process());
}



/* This is run before EACH TEST */
void setUp(void)
{
}


void tearDown(void)
{
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_fifoc);
    RUN_TEST(test_fifor);
    RUN_TEST(test_logger);
    return UNITY_END();
}