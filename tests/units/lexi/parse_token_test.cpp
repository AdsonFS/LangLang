#include "../../../src/lexi/lexi_scanner.h"
#include "./lexi_fixture_test.h"
#include <gtest/gtest.h>


TEST_F(LexiUnitTest, lexi_token_identifier_dot_comma_colon) {
  set_file_content("  person.name\t\t, \nage:token");

  Token token_person = scanner->nextToken();
  EXPECT_TRUE(match(token_person, Token(TokenType::TK_IDENTIFIER, "person")));

  Token token_dot = scanner->nextToken();
  EXPECT_TRUE(match(token_dot, Token(TokenType::TK_DOT, ".")));

  Token token_name = scanner->nextToken();
  EXPECT_TRUE(match(token_name, Token(TokenType::TK_IDENTIFIER, "name")));

  Token token_comma = scanner->nextToken();
  EXPECT_TRUE(match(token_comma, Token(TokenType::TK_COMMA, ",")));

  Token token_age = scanner->nextToken();
  EXPECT_TRUE(match(token_age, Token(TokenType::TK_IDENTIFIER, "age")));

  Token token_colon = scanner->nextToken();
  EXPECT_TRUE(match(token_colon, Token(TokenType::TK_COLON, ":")));

  Token token_token = scanner->nextToken();
  EXPECT_TRUE(match(token_token, Token(TokenType::TK_IDENTIFIER, "token")));

  Token token_eof = scanner->nextToken();
  EXPECT_TRUE(match(token_eof, Token(TokenType::TK_EOF, "EOF")));
}

TEST_F(LexiUnitTest, lexi_token_string_number_arrow) {
  set_file_content("\"string\", 123  \n\n  ->");

  Token token_string = scanner->nextToken();
  EXPECT_TRUE(match(token_string, Token(TokenType::TK_STRING, "string")));

  Token token_comma = scanner->nextToken();
  EXPECT_TRUE(match(token_comma, Token(TokenType::TK_COMMA, ",")));

  Token token_number = scanner->nextToken();
  EXPECT_TRUE(match(token_number, Token(TokenType::TK_NUMBER, "123")));

  Token token_arrow = scanner->nextToken();
  EXPECT_TRUE(match(token_arrow, Token(TokenType::TK_ARROW, "->")));

  Token token_eof = scanner->nextToken();
  EXPECT_TRUE(match(token_eof, Token(TokenType::TK_EOF, "EOF")));
}

TEST_F(LexiUnitTest, lexi_token_operator_parentheses_inputstream_outputstream) {
  set_file_content("+-*/%  ( )  <<  >>");

  Token token_plus = scanner->nextToken();
  EXPECT_TRUE(match(token_plus, Token(TokenType::TK_OPERATOR, "+")));

  Token token_minus = scanner->nextToken();
  EXPECT_TRUE(match(token_minus, Token(TokenType::TK_OPERATOR, "-")));

  Token token_mult = scanner->nextToken();
  EXPECT_TRUE(match(token_mult, Token(TokenType::TK_OPERATOR, "*")));

  Token token_div = scanner->nextToken();
  EXPECT_TRUE(match(token_div, Token(TokenType::TK_OPERATOR, "/")));

  Token token_mod = scanner->nextToken();
  EXPECT_TRUE(match(token_mod, Token(TokenType::TK_OPERATOR, "%")));

  Token token_open_parentheses = scanner->nextToken();
  EXPECT_TRUE(match(token_open_parentheses, Token(TokenType::TK_PARENTHESES, "(")));

  Token token_close_parentheses = scanner->nextToken();
  EXPECT_TRUE(match(token_close_parentheses, Token(TokenType::TK_PARENTHESES, ")")));

  Token token_inputstream = scanner->nextToken();
  EXPECT_TRUE(match(token_inputstream, Token(TokenType::TK_INPUTSTREAM, "<<")));

  Token token_outputstream = scanner->nextToken();
  EXPECT_TRUE(match(token_outputstream, Token(TokenType::TK_OUTPUTSTREAM, ">>")));

  Token token_eof = scanner->nextToken();
  EXPECT_TRUE(match(token_eof, Token(TokenType::TK_EOF, "EOF")));
}

TEST_F(LexiUnitTest, lexi_token_all_reserved_words) {
  set_file_content("class void var func number string return if else while for");
  std::string reserved_words[] = {"class", "void", "var", "func", "number", "string",
                                  "return", "if", "else", "while", "for"};

  for (std::string word : reserved_words) {
    Token token = scanner->nextToken();
    EXPECT_TRUE(match(token, Token(TokenType::TK_RESERVED_WORD, word)));
  }

  Token token_eof = scanner->nextToken();
  EXPECT_TRUE(match(token_eof, Token(TokenType::TK_EOF, "EOF")));
}

TEST_F(LexiUnitTest, lexi_token_assignment_curly_braces_comparator_logical_operator) {
  set_file_content(":= {}  < >  && ||");

  Token token_assignment = scanner->nextToken();
  EXPECT_TRUE(match(token_assignment, Token(TokenType::TK_ASSIGNMENT, ":=")));

  Token token_open_curly_braces = scanner->nextToken();
  EXPECT_TRUE(match(token_open_curly_braces, Token(TokenType::TK_CURLY_BRACES, "{")));

  Token token_close_curly_braces = scanner->nextToken();
  EXPECT_TRUE(match(token_close_curly_braces, Token(TokenType::TK_CURLY_BRACES, "}")));

  Token token_less = scanner->nextToken();
  EXPECT_TRUE(match(token_less, Token(TokenType::TK_COMPARATOR, "<")));

  Token token_greater = scanner->nextToken();
  EXPECT_TRUE(match(token_greater, Token(TokenType::TK_COMPARATOR, ">")));

  Token token_and = scanner->nextToken();
  EXPECT_TRUE(match(token_and, Token(TokenType::TK_LOGICAL_OPERATOR, "&&")));

  Token token_or = scanner->nextToken();
  EXPECT_TRUE(match(token_or, Token(TokenType::TK_LOGICAL_OPERATOR, "||")));

  Token token_eof = scanner->nextToken();
  EXPECT_TRUE(match(token_eof, Token(TokenType::TK_EOF, "EOF")));
}

TEST_F(LexiUnitTest, lexi_token_equality_operator_semicolon_comment) {
  set_file_content("== ;  \n\n  <>  !=");

  Token token_equal = scanner->nextToken();
  EXPECT_TRUE(match(token_equal, Token(TokenType::TK_EQUALITY_OPERATOR, "==")));

  Token token_semicolon = scanner->nextToken();
  EXPECT_TRUE(match(token_semicolon, Token(TokenType::TK_SEMICOLON, ";")));

  Token token_comment = scanner->nextToken();
  EXPECT_TRUE(match(token_comment, Token(TokenType::TK_EOF, "EOF"))); // comment is ignored
}

TEST_F(LexiUnitTest, lexi_token_panic_mode) {
  set_file_content("func 123 ; \n\n  456 789; ||");

  Token token_func = scanner->nextToken();
  EXPECT_TRUE(match(token_func, Token(TokenType::TK_RESERVED_WORD, "func")));

  scanner->panicMode();

  Token token_number = scanner->nextToken();
  EXPECT_TRUE(match(token_number, Token(TokenType::TK_NUMBER, "456")));

  scanner->panicMode();

  Token token_or = scanner->nextToken();
  EXPECT_TRUE(match(token_or, Token(TokenType::TK_LOGICAL_OPERATOR, "||")));

  Token token_eof = scanner->nextToken();
  EXPECT_TRUE(match(token_eof, Token(TokenType::TK_EOF, "EOF")));
}
