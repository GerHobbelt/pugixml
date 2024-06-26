#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef PUGIXML_NO_XPATH

#include "test.hpp"

#include <string>
#include <algorithm>

using namespace pugi;

TEST_XML(xpath_xalan_string_1, "<doc a='test'>ENCYCLOPEDIA</doc>")
{
	xml_node c;

	CHECK_XPATH_NUMBER(c, STR("string-length('This is a test')"), 14);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('ENCYCLOPEDIA', 'ENCY')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("contains('ENCYCLOPEDIA', 'CYCL')"), true);
	CHECK_XPATH_STRING(c, STR("substring-before('1999/04/01', '/')"), STR("1999"));
	CHECK_XPATH_STRING(c, STR("substring-after('1999/04/01', '/')"), STR("04/01"));
	CHECK_XPATH_STRING(c, STR("normalize-space('\t\n\r\n    ab\n     cd\t\n\r\n    ef\t\n\r ')"), STR("ab cd ef"));
	CHECK_XPATH_STRING(c, STR("translate(\"bar\",\"abc\",\"ABC\")"), STR("BAr"));
	CHECK_XPATH_STRING(c, STR("concat(\"x\",\"yz\")"), STR("xyz"));
	CHECK_XPATH_STRING(c, STR("substring('1999/04/01', 1, 4)"), STR("1999"));
	CHECK_XPATH_STRING(c, STR("substring('12345', 1.5, 2.6)"), STR("234"));
	CHECK_XPATH_STRING(c, STR("substring('12345', 0, 3)"), STR("12"));
	CHECK_XPATH_STRING(c, STR("substring('12345', 0 div 0, 3)"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring('12345', 1, 0 div 0)"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring('12345', -42, 1 div 0)"), STR("12345"));
	CHECK_XPATH_STRING(c, STR("substring('12345', -1 div 0, 1 div 0)"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring(foo, 12, 3)"), STR(""));
	CHECK_XPATH_STRING(c, STR("string(foo)"), STR(""));
	CHECK_XPATH_STRING(c, STR("string(0)"), STR("0"));
	CHECK_XPATH_STRING(c, STR("string(2)"), STR("2"));
	CHECK_XPATH_STRING(c, STR("string('test')"), STR("test"));
	CHECK_XPATH_STRING(c, STR("string('')"), STR(""));
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('ENCYCLOPEDIA', 'EN')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('ENCYCLOPEDIA', 'en')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('ab', 'abc')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('abc', 'bc')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('abc', '')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('', '')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('', 'abc')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with('true()', 'tr')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("starts-with(foo, 'EN')"), false);
	CHECK_XPATH_BOOLEAN(doc, STR("starts-with(doc, 'EN')"), true);
	CHECK_XPATH_BOOLEAN(doc, STR("starts-with(doc/@a, 'EN')"), false);
	CHECK_XPATH_BOOLEAN(doc, STR("starts-with(doc/@a, 'te')"), true);
}

TEST_XML_FLAGS(xpath_xalan_string_2, "<doc>\n  <av>\n    <a>\n      <b>b</b>\n      <c>c</c>\n      <d>d</d>\n      <e>e</e>\n    </a>\n    <v>\n      <w>w</w>\n      <x>x</x>\n      <y>y</y>\n      <z>z</z>\n    </v>\n  </av>\n</doc>", parse_default | parse_ws_pcdata)
{
	CHECK_XPATH_STRING(doc, STR("string(doc/av//*)"), STR("\n      b\n      c\n      d\n      e\n    "));
	CHECK_XPATH_STRING(doc, STR("normalize-space(string(doc/av//*))"), STR("b c d e"));
	CHECK_XPATH_STRING(doc, STR("normalize-space('This       is       a       test')"), STR("This is a test"));
}

TEST_XML(xpath_xalan_string_3, "<doc a='test'>ENCYCLOPEDIA</doc>")
{
	xml_node c;

	CHECK_XPATH_BOOLEAN(c, STR("contains('ENCYCLOPEDIA', 'TEST')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("contains('ENCYCLOPEDIA', 'CYCL')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("contains('ENCYCLOPEDIA', 'cycl')"), false);
	CHECK_XPATH_BOOLEAN(doc, STR("contains(concat(.,'BC'),concat('A','B','C'))"), true);
	CHECK_XPATH_BOOLEAN(c, STR("contains('ab', 'abc')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("contains('abc', 'bc')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("contains('abc', 'bcd')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("contains('abc', '')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("contains('', '')"), true);
	CHECK_XPATH_BOOLEAN(c, STR("contains('', 'abc')"), false);
	CHECK_XPATH_BOOLEAN(c, STR("contains('true()', 'e')"), true);
	CHECK_XPATH_BOOLEAN(doc, STR("contains(., 'CYCL')"), true);
	CHECK_XPATH_BOOLEAN(doc, STR("contains(., 'TEST')"), false);
	CHECK_XPATH_BOOLEAN(doc, STR("contains(doc/@a, 'es')"), true);
	CHECK_XPATH_BOOLEAN(doc, STR("contains(doc/@a, 'T')"), false);
	CHECK_XPATH_STRING(c, STR("substring-before('ENCYCLOPEDIA', '/')"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring-before('ENCYCLOPEDIA', 'C')"), STR("EN"));
	CHECK_XPATH_STRING(c, STR("substring-before('ENCYCLOPEDIA', 'c')"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring-before('ENCYCLOPEDIA', '')"), STR(""));
	CHECK_XPATH_STRING(doc, STR("substring-before(., '/')"), STR(""));
	CHECK_XPATH_STRING(doc, STR("substring-before(., 'C')"), STR("EN"));
	CHECK_XPATH_STRING(doc, STR("substring-before(foo, '')"), STR(""));
	CHECK_XPATH_STRING(doc, STR("substring-before(doc/@a, '/')"), STR(""));
	CHECK_XPATH_STRING(doc, STR("substring-before(doc/@a, 'e')"), STR("t"));
	CHECK_XPATH_STRING(doc, STR("substring-before(doc/@a, 't')"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring-after('ENCYCLOPEDIA', '/')"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring-after('ENCYCLOPEDIA', 'C')"), STR("YCLOPEDIA"));
	CHECK_XPATH_STRING(c, STR("substring-after('ENCYCLOPEDIA', 'c')"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring-after('ENCYCLOPEDIA', '')"), STR("ENCYCLOPEDIA"));
	CHECK_XPATH_STRING(doc, STR("substring-after(., '/')"), STR(""));
	CHECK_XPATH_STRING(doc, STR("substring-after(., 'C')"), STR("YCLOPEDIA"));
	CHECK_XPATH_STRING(doc, STR("substring-after(foo, '')"), STR(""));
	CHECK_XPATH_STRING(doc, STR("substring-after(doc/@a, '/')"), STR(""));
	CHECK_XPATH_STRING(doc, STR("substring-after(doc/@a, 'e')"), STR("st"));
	CHECK_XPATH_STRING(doc, STR("substring-after(doc/@a, 't')"), STR("est"));
	CHECK_XPATH_STRING(doc, STR("substring-after(doc/@a, 'st')"), STR(""));
}

TEST_XML(xpath_xalan_string_4, "<doc><a>a</a><b>b</b><c>c</c><d>d</d><e>ef</e><f attr='whatsup'>what's up</f></doc><cd><![CDATA[qua'lit\"y]]></cd>")
{
	xml_node c;

	CHECK_XPATH_STRING(c, STR("translate('BAR','abc','ABC')"), STR("BAR"));
	CHECK_XPATH_STRING(c, STR("translate('bar','RAB','xyz')"), STR("bar"));
	CHECK_XPATH_STRING(c, STR("translate('BAR','Rab','TxX')"), STR("BAT"));
	CHECK_XPATH_STRING(c, STR("translate('zzaaazzz','abcz','ABC')"), STR("AAA"));
	CHECK_XPATH_STRING(c, STR("translate('ddaaadddd','abcd','ABCxy')"), STR("xxAAAxxxx"));
	CHECK_XPATH_STRING(c, STR("concat('a','b','c','d','ef')"), STR("abcdef"));
	CHECK_XPATH_STRING(c, STR("concat(a, b)"), STR(""));
	CHECK_XPATH_STRING(doc.child(STR("doc")), STR("concat(a, b)"), STR("ab"));
	CHECK_XPATH_STRING(doc.child(STR("doc")), STR("concat(a, b, c, d, e)"), STR("abcdef"));
	CHECK_XPATH_STRING(c, STR("concat('cd','34')"), STR("cd34"));
	CHECK_XPATH_STRING(c, STR("concat('cd',34)"), STR("cd34"));
	CHECK_XPATH_STRING(c, STR("concat('bc',string(23))"), STR("bc23"));
	CHECK_XPATH_STRING(c, STR("concat(a,34)"), STR("34"));
	CHECK_XPATH_STRING(doc.child(STR("doc")), STR("concat(a,34)"), STR("a34"));
	CHECK_XPATH_STRING(c, STR("concat(false(),'ly')"), STR("falsely"));
	CHECK_XPATH_FAIL(STR("concat(/*)"));
	CHECK_XPATH_STRING(doc.child(STR("doc")), STR("concat(/*, '')"), STR("abcdefwhat's up"));
	CHECK_XPATH_STRING(doc.child(STR("doc")), STR("concat(/*, /*[@attr='whatsup'])"), STR("abcdefwhat's up"));
	CHECK_XPATH_STRING(doc.child(STR("doc")), STR("concat(/*, //*[@attr='whatsup'])"), STR("abcdefwhat's upwhat's up"));
	CHECK_XPATH_STRING(c, STR("substring('ENCYCLOPEDIA', 8, 3)"), STR("PED"));
	CHECK_XPATH_STRING(c, STR("substring('ENCYCLOPEDIA', 8)"), STR("PEDIA"));
	CHECK_XPATH_STRING(c, STR("substring('abcdefghijk',0 div 0, 5)"), STR(""));
	CHECK_XPATH_STRING(c, STR("substring('abcdefghijk',4, 6)"), STR("defghi"));
	CHECK_XPATH_STRING(c, STR("substring('1999/04/01', 1, 0)"), STR(""));
	CHECK_XPATH_STRING(c, STR("translate(normalize-space('     bar       fly        '), ' ', '_')"), STR("bar_fly"));
	CHECK_XPATH_STRING(c, STR("translate('barter','abe','bao')"), STR("abrtor"));
	CHECK_XPATH_STRING(c, STR("translate('barbarity', 'aeiouy', '******')"), STR("b*rb*r*t*"));
	CHECK_XPATH_STRING(doc, STR("translate(cd, concat(\"aqu'\", '\"eos'), 'AQU-+EOS')"), STR("QUA-lit+y"));
	CHECK_XPATH_STRING(c, STR("translate('quan+ti-ty', 'AQU-+EOS', concat(\"aqu'\", '\"eos'))"), STR("quan\"ti'ty"));
}

static const char_t* number_to_string_unsafe(int number)
{
	static char_t buffer[16];

	// construct number in reverse
	char_t* it = buffer;

	while (number)
	{
		*it++ = static_cast<char_t>('0' + number % 10);
		number /= 10;
	}

	// zero terminate
	*it = 0;

	// reverse to get final result
	std::reverse(buffer, it);

	return buffer;
}

TEST(xpath_xalan_string_5)
{
	const int count = 1000;

	std::basic_string<char_t> query;
	query.reserve(17 * count);

	query += STR("concat(");

	for (int i = 1; i < count; ++i)
	{
		query += STR("concat('t',");
		query += number_to_string_unsafe(i);
		query += STR("), ");
	}

	query += STR("'')");

	std::basic_string<char_t> expected;
	expected.reserve(4 * count);

	for (int j = 1; j < count; ++j)
	{
		expected += STR("t");
		expected += number_to_string_unsafe(j);
	}

	CHECK_XPATH_STRING(xml_node(), query.c_str(), expected.c_str());
}

TEST(xpath_xalan_string_6)
{
	xml_node c;

	CHECK_XPATH_STRING(c, STR("string(1)"), STR("1"));
	CHECK_XPATH_STRING(c, STR("string(12)"), STR("12"));
	CHECK_XPATH_STRING(c, STR("string(123)"), STR("123"));
	CHECK_XPATH_STRING(c, STR("string(1234)"), STR("1234"));
	CHECK_XPATH_STRING(c, STR("string(12345)"), STR("12345"));
	CHECK_XPATH_STRING(c, STR("string(123456)"), STR("123456"));
	CHECK_XPATH_STRING(c, STR("string(1234567)"), STR("1234567"));
	CHECK_XPATH_STRING(c, STR("string(12345678)"), STR("12345678"));
	CHECK_XPATH_STRING(c, STR("string(123456789)"), STR("123456789"));
	CHECK_XPATH_STRING(c, STR("string(1234567890)"), STR("1234567890"));
	CHECK_XPATH_STRING(c, STR("string(12345678901)"), STR("12345678901"));
	CHECK_XPATH_STRING(c, STR("string(123456789012)"), STR("123456789012"));
	CHECK_XPATH_STRING(c, STR("string(1234567890123)"), STR("1234567890123"));
	CHECK_XPATH_STRING(c, STR("string(12345678901234)"), STR("12345678901234"));
	CHECK_XPATH_STRING(c, STR("string(123456789012345)"), STR("123456789012345"));
	CHECK_XPATH_STRING(c, STR("string(1234567890123456)"), STR("1234567890123456"));
	CHECK_XPATH_STRING(c, STR("string(-1)"), STR("-1"));
	CHECK_XPATH_STRING(c, STR("string(-12)"), STR("-12"));
	CHECK_XPATH_STRING(c, STR("string(-123)"), STR("-123"));
	CHECK_XPATH_STRING(c, STR("string(-1234)"), STR("-1234"));
	CHECK_XPATH_STRING(c, STR("string(-12345)"), STR("-12345"));
	CHECK_XPATH_STRING(c, STR("string(-123456)"), STR("-123456"));
	CHECK_XPATH_STRING(c, STR("string(-1234567)"), STR("-1234567"));
	CHECK_XPATH_STRING(c, STR("string(-12345678)"), STR("-12345678"));
	CHECK_XPATH_STRING(c, STR("string(-123456789)"), STR("-123456789"));
	CHECK_XPATH_STRING(c, STR("string(-1234567890)"), STR("-1234567890"));
	CHECK_XPATH_STRING(c, STR("string(-12345678901)"), STR("-12345678901"));
	CHECK_XPATH_STRING(c, STR("string(-123456789012)"), STR("-123456789012"));
	CHECK_XPATH_STRING(c, STR("string(-1234567890123)"), STR("-1234567890123"));
	CHECK_XPATH_STRING(c, STR("string(-12345678901234)"), STR("-12345678901234"));
	CHECK_XPATH_STRING(c, STR("string(-123456789012345)"), STR("-123456789012345"));
	CHECK_XPATH_STRING(c, STR("string(-1234567890123456)"), STR("-1234567890123456"));
}

#if 0 // $ this test requires round-to-nearest behavior in string->number conversion during parsing; atof gives us truncation
TEST(xpath_xalan_string_6_rounding)
{
	xml_node c;

	CHECK_XPATH_STRING(c, STR("string(12345678901234567)"), STR("12345678901234568"));
	CHECK_XPATH_STRING(c, STR("string(123456789012345678)"), STR("123456789012345680"));
	CHECK_XPATH_STRING(c, STR("string(-12345678901234567)"), STR("-12345678901234568"));
	CHECK_XPATH_STRING(c, STR("string(-123456789012345678)"), STR("-123456789012345680"));
}
#endif

TEST(xpath_xalan_string_7)
{
	xml_node c;

	CHECK_XPATH_STRING(c, STR("string(.1)"), STR("0.1"));
	CHECK_XPATH_STRING(c, STR("string(.01)"), STR("0.01"));
	CHECK_XPATH_STRING(c, STR("string(.012)"), STR("0.012"));
	CHECK_XPATH_STRING(c, STR("string(.0123)"), STR("0.0123"));
	CHECK_XPATH_STRING(c, STR("string(.01234)"), STR("0.01234"));
	CHECK_XPATH_STRING(c, STR("string(.012345)"), STR("0.012345"));
	CHECK_XPATH_STRING(c, STR("string(.0123456)"), STR("0.0123456"));
	CHECK_XPATH_STRING(c, STR("string(.01234567)"), STR("0.01234567"));
	CHECK_XPATH_STRING(c, STR("string(.012345678)"), STR("0.012345678"));
	CHECK_XPATH_STRING(c, STR("string(.0123456789)"), STR("0.0123456789"));
	CHECK_XPATH_STRING(c, STR("string(.10123456789)"), STR("0.10123456789"));
	CHECK_XPATH_STRING(c, STR("string(.101234567892)"), STR("0.101234567892"));
	CHECK_XPATH_STRING(c, STR("string(.1012345678923)"), STR("0.1012345678923"));
	CHECK_XPATH_STRING(c, STR("string(.10123456789234)"), STR("0.10123456789234"));
	CHECK_XPATH_STRING(c, STR("string(.101234567892345)"), STR("0.101234567892345"));
	CHECK_XPATH_STRING(c, STR("string(.1012345678923456)"), STR("0.1012345678923456"));
	CHECK_XPATH_STRING(c, STR("string(-.1)"), STR("-0.1"));
	CHECK_XPATH_STRING(c, STR("string(-.01)"), STR("-0.01"));
	CHECK_XPATH_STRING(c, STR("string(-.012)"), STR("-0.012"));
	CHECK_XPATH_STRING(c, STR("string(-.0123)"), STR("-0.0123"));
	CHECK_XPATH_STRING(c, STR("string(-.01234)"), STR("-0.01234"));
	CHECK_XPATH_STRING(c, STR("string(-.012345)"), STR("-0.012345"));
	CHECK_XPATH_STRING(c, STR("string(-.0123456)"), STR("-0.0123456"));
	CHECK_XPATH_STRING(c, STR("string(-.01234567)"), STR("-0.01234567"));
	CHECK_XPATH_STRING(c, STR("string(-.012345678)"), STR("-0.012345678"));
	CHECK_XPATH_STRING(c, STR("string(-.0123456789)"), STR("-0.0123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.10123456789)"), STR("-0.10123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.101234567892)"), STR("-0.101234567892"));
	CHECK_XPATH_STRING(c, STR("string(-.1012345678923)"), STR("-0.1012345678923"));
	CHECK_XPATH_STRING(c, STR("string(-.10123456789234)"), STR("-0.10123456789234"));
	CHECK_XPATH_STRING(c, STR("string(-.101234567892345)"), STR("-0.101234567892345"));
	CHECK_XPATH_STRING(c, STR("string(-.1012345678923456)"), STR("-0.1012345678923456"));
}

#if 0 // $ this test requires 16 decimal digits of mantissa in number->string conversion; we have 15 since only 15 is guaranteed, and 16 introduces 'garbage' digits in common cases like 0.4
TEST(xpath_xalan_string_7_precision)
{
	xml_node c;

	CHECK_XPATH_STRING(c, STR("string(.10123456789234567)"), STR("0.10123456789234567"));
	CHECK_XPATH_STRING(c, STR("string(.101234567892345678)"), STR("0.10123456789234568"));
	CHECK_XPATH_STRING(c, STR("string(.1012345678923456789)"), STR("0.10123456789234568"));
	CHECK_XPATH_STRING(c, STR("string(.10123456789234567893)"), STR("0.10123456789234568"));
	CHECK_XPATH_STRING(c, STR("string(-.10123456789234567)"), STR("-0.10123456789234567"));
	CHECK_XPATH_STRING(c, STR("string(-.101234567892345678)"), STR("-0.10123456789234568"));
	CHECK_XPATH_STRING(c, STR("string(-.1012345678923456789)"), STR("-0.10123456789234568"));
	CHECK_XPATH_STRING(c, STR("string(-.10123456789234567893)"), STR("-0.10123456789234568"));
}
#endif

TEST(xpath_xalan_string_8)
{
	xml_node c;

	// $ originally all last digits were 5's; a fully compliant implementation should correctly convert those as well,
	// however some of these failed because of atof truncation
	CHECK_XPATH_STRING(c, STR("string(9.87654321012344)"), STR("9.87654321012344"));
	CHECK_XPATH_STRING(c, STR("string(98.7654321012345)"), STR("98.7654321012345"));
	CHECK_XPATH_STRING(c, STR("string(987.654321012345)"), STR("987.654321012345"));
	CHECK_XPATH_STRING(c, STR("string(9876.54321012344)"), STR("9876.54321012344"));
	CHECK_XPATH_STRING(c, STR("string(98765.4321012345)"), STR("98765.4321012345"));
	CHECK_XPATH_STRING(c, STR("string(987654.321012345)"), STR("987654.321012345"));
	CHECK_XPATH_STRING(c, STR("string(9876543.21012345)"), STR("9876543.21012345"));
	CHECK_XPATH_STRING(c, STR("string(98765432.1012345)"), STR("98765432.1012345"));
	CHECK_XPATH_STRING(c, STR("string(987654321.012345)"), STR("987654321.012345"));
	CHECK_XPATH_STRING(c, STR("string(9876543210.12344)"), STR("9876543210.12344"));
	CHECK_XPATH_STRING(c, STR("string(98765432101.2345)"), STR("98765432101.2345"));
	CHECK_XPATH_STRING(c, STR("string(987654321012.345)"), STR("987654321012.345"));
	CHECK_XPATH_STRING(c, STR("string(9876543210123.43)"), STR("9876543210123.43"));
	CHECK_XPATH_STRING(c, STR("string(98765432101234.5)"), STR("98765432101234.5"));

	CHECK_XPATH_STRING(c, STR("string(-9.87654321012344)"), STR("-9.87654321012344"));
	CHECK_XPATH_STRING(c, STR("string(-98.7654321012345)"), STR("-98.7654321012345"));
	CHECK_XPATH_STRING(c, STR("string(-987.654321012345)"), STR("-987.654321012345"));
	CHECK_XPATH_STRING(c, STR("string(-9876.54321012344)"), STR("-9876.54321012344"));
	CHECK_XPATH_STRING(c, STR("string(-98765.4321012345)"), STR("-98765.4321012345"));
	CHECK_XPATH_STRING(c, STR("string(-987654.321012345)"), STR("-987654.321012345"));
	CHECK_XPATH_STRING(c, STR("string(-9876543.21012345)"), STR("-9876543.21012345"));
	CHECK_XPATH_STRING(c, STR("string(-98765432.1012345)"), STR("-98765432.1012345"));
	CHECK_XPATH_STRING(c, STR("string(-987654321.012345)"), STR("-987654321.012345"));
	CHECK_XPATH_STRING(c, STR("string(-9876543210.12344)"), STR("-9876543210.12344"));
	CHECK_XPATH_STRING(c, STR("string(-98765432101.2345)"), STR("-98765432101.2345"));
	CHECK_XPATH_STRING(c, STR("string(-987654321012.345)"), STR("-987654321012.345"));
	CHECK_XPATH_STRING(c, STR("string(-9876543210123.43)"), STR("-9876543210123.43"));
	CHECK_XPATH_STRING(c, STR("string(-98765432101234.5)"), STR("-98765432101234.5"));
}

TEST(xpath_xalan_string_9)
{
	xml_node c;

	CHECK_XPATH_STRING(c, STR("string(.123456789)"), STR("0.123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0123456789)"), STR("0.0123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00123456789)"), STR("0.00123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000123456789)"), STR("0.000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000123456789)"), STR("0.0000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000123456789)"), STR("0.00000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000123456789)"), STR("0.000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000123456789)"), STR("0.0000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000123456789)"), STR("0.00000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000123456789)"), STR("0.000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000123456789)"), STR("0.0000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000123456789)"), STR("0.00000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000123456789)"), STR("0.000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000123456789)"), STR("0.0000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000123456789)"), STR("0.00000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000123456789)"), STR("0.000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000123456789)"), STR("0.0000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000123456789)"), STR("0.00000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000123456789)"), STR("0.000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000123456789)"), STR("0.0000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000000123456789)"), STR("0.00000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000000123456789)"), STR("0.000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000000123456789)"), STR("0.0000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000000000123456789)"), STR("0.00000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000000000123456789)"), STR("0.000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000000000123456789)"), STR("0.0000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000000000000123456789)"), STR("0.00000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000000000000123456789)"), STR("0.000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000000000000123456789)"), STR("0.0000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000000000000000123456789)"), STR("0.00000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000000000000000123456789)"), STR("0.000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000000000000000123456789)"), STR("0.0000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000000000000000000123456789)"), STR("0.00000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000000000000000000123456789)"), STR("0.000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000000000000000000123456789)"), STR("0.0000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000000000000000000000123456789)"), STR("0.00000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000000000000000000000123456789)"), STR("0.000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000000000000000000000123456789)"), STR("0.0000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.00000000000000000000000000000000000000123456789)"), STR("0.00000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.000000000000000000000000000000000000000123456789)"), STR("0.000000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(.0000000000000000000000000000000000000000123456789)"), STR("0.0000000000000000000000000000000000000000123456789"));

	CHECK_XPATH_STRING(c, STR("string(-.123456789)"), STR("-0.123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0123456789)"), STR("-0.0123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00123456789)"), STR("-0.00123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000123456789)"), STR("-0.000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000123456789)"), STR("-0.0000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000123456789)"), STR("-0.00000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000123456789)"), STR("-0.000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000123456789)"), STR("-0.0000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000123456789)"), STR("-0.00000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000123456789)"), STR("-0.000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000123456789)"), STR("-0.0000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000123456789)"), STR("-0.00000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000123456789)"), STR("-0.000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000123456789)"), STR("-0.0000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000123456789)"), STR("-0.00000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000123456789)"), STR("-0.000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000123456789)"), STR("-0.0000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000123456789)"), STR("-0.00000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000123456789)"), STR("-0.000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000123456789)"), STR("-0.0000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000000123456789)"), STR("-0.00000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000000123456789)"), STR("-0.000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000000123456789)"), STR("-0.0000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000000000123456789)"), STR("-0.00000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000000000123456789)"), STR("-0.000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000000000123456789)"), STR("-0.0000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000000000000123456789)"), STR("-0.00000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000000000000123456789)"), STR("-0.000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000000000000123456789)"), STR("-0.0000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000000000000000123456789)"), STR("-0.00000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000000000000000123456789)"), STR("-0.000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000000000000000123456789)"), STR("-0.0000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000000000000000000123456789)"), STR("-0.00000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000000000000000000123456789)"), STR("-0.000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000000000000000000123456789)"), STR("-0.0000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000000000000000000000123456789)"), STR("-0.00000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000000000000000000000123456789)"), STR("-0.000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000000000000000000000123456789)"), STR("-0.0000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.00000000000000000000000000000000000000123456789)"), STR("-0.00000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.000000000000000000000000000000000000000123456789)"), STR("-0.000000000000000000000000000000000000000123456789"));
	CHECK_XPATH_STRING(c, STR("string(-.0000000000000000000000000000000000000000123456789)"), STR("-0.0000000000000000000000000000000000000000123456789"));
}

#endif
