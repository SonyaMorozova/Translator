#include "translator.h"
#include <gtest.h>

TEST(Calculate, calculation_is_correct) {
	Translator translator("((6*2-89*9)+2+3)*11");
	translator.infix_to_postfix();
	ASSERT_EQ(translator.calculate(), -8624);
}

TEST(Calculate, can_get_postfix) {
	Translator translator("((6*2-89*9)+2+3)*11");
	translator.infix_to_postfix();
	ASSERT_EQ(translator.get_postfix(), "62*899*-2+3+11*");
}

TEST(Calculate, can_get_infix) {
	Translator translator("((6*2-89*9)+2+3)*11");
	ASSERT_EQ(translator.get_infox(), "((6*2-89*9)+2+3)*11");
}

TEST(Calculate, cant_division_by_zero) {
	Translator translator("11/0");
	ASSERT_ANY_THROW(translator.calculate());
}

TEST(Calculate, throw_if_unknown_symbol) {
	Translator translator("a/b");
	ASSERT_ANY_THROW(translator.calculate());
}

TEST(Calculate, throw_if_not_correct_input) {
	Translator translator1(")5+1");
	Translator translator2("((5+1)");
	Translator translator3("*(5+1)");
	Translator translator4("5++1");
	Translator translator5("5+");
	ASSERT_ANY_THROW(translator1.calculate());
	ASSERT_ANY_THROW(translator2.calculate());
	ASSERT_ANY_THROW(translator3.calculate());
	ASSERT_ANY_THROW(translator4.calculate());
	ASSERT_ANY_THROW(translator5.calculate());

}



