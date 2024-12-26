#include "translator.h"
#include <gtest.h>

TEST(Calculate, can_correct_work_calculate_Translator) {
	Translator translator("((6.2*2-89*9)+2+3)*11");
	ASSERT_EQ( translator.calculate(), -8619.6);	
}

TEST(Calculate, can_get_postfix_Translator) {
	Translator translator("((6*2-89*9)+2+3)*11");
	ASSERT_EQ(translator.get_postfix(), "62*899*-2+3+11*");
}

TEST(Calculate, can_get_infix_Translator) {
	Translator translator("((6*2-89*9)+2+3)*11");
	ASSERT_EQ(translator.get_infix(), "((6*2-89*9)+2+3)*11");
}

TEST(Calculate, throw_division_by_zero_Translator) { 
	Translator translator("11/0");
	ASSERT_ANY_THROW(translator.calculate());
}

TEST(Calculate, throw_not_correct_input_2_Translator) {
	Translator translator("((5+1)");
	ASSERT_ANY_THROW(translator.calculate());
}

TEST(Calculate, throw_not_correct_input_3_Translator) {
	Translator translator("*(5+1)");
	ASSERT_ANY_THROW(translator.calculate());
}

TEST(Calculate, throw_not_correct_input_4_Translator) {
	Translator translator("5++1");
	ASSERT_ANY_THROW(translator.calculate());
}
