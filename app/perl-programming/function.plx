#! /usr/bin/perl
#1.函数的返回值为最后一次运算的结果。
#2.子函数里的最后一条语句不能是print打印语句。
#3.函数声明用sub关键字开关，函数名 及中括号的函数体
#4.函数调用 &function_name.
#5.函数调用可以先于函数声明。
#6.函数参数:&function_name(arg1,arg2)

#此句强制使用一些严格的良好的编程风格
use strict;

my($n) = 10;
sub marine {
	$n += 1;
}

my($ret) = &marine;
$ret = &marine;
$ret = &marine;
print "ret=$ret\n";

#_[0]、_[1]主私有数据@_的第一个元素和第二个元素
sub fun_max {
	if ($_[0] > $_[1]) {
		$_[0];
	} else {
		$_[1];
	}
}
$ret = &fun_max(2, 3);
print "fun_max(2,3) result is $ret\n";

# @_是子函数里的一个私有数组
# my关键字用来声明变量为子函数里私有变量 （实际上是可以在任何语句块声明私有变量）
sub fun_max_my {
#	my($m, $n);
#	($m, $n) = @_;
	my($m, $n) = @_;  #与my($m, $n);($m, $n) = @_;两语句等价
	if ($m > $n) {$m} else {$n}
}
$ret = &fun_max_my(4, 5);
print "fun_max_my(4,5) result is $ret\n";

# 注意：此函数为可变参数
# @_为子函数的私有数组，用来保存函数参数
# $_为foreach的默认变量
sub fun_max_far {
	my($max_so_far) = shift @_;
	foreach (@_) {
		if($_ > $max_so_far) {
			$max_so_far = $_;
		}
	}
	$max_so_far;
}
my(@array) = qw\1 22 3 55 100 25\;
my($ret_max) = &fun_max_far(@array);
print "[ @array ] max element is $ret_max\n";

#return 关键字的用法
sub which_element_is {
	my($what, @array) = @_;
	foreach (0..$#array) {
		if($what eq $array[$_]) {
			return $_;
		}
	}
	return -1;
}
my(@names) = qw(fred barney dino wilma pebbles);
my($result) = &which_element_is('dino', @names);
print "element [ dino ] in the array [ @names ] position is $result.\n";

# ".."为递增操作
sub list_from_fred_to_barney {
	my($m, $n) = @_;
	if ($m < $n) {
		$m..$n;
	} else {
		reverse ($n..$m);
	}
}
my(@arr) = qw/50 40/;
my(@result) = &list_from_fred_to_barney(@arr);
print "[ @arr ] all value is [ @result ]\n";

#求和
sub total {
	my($sum) = 0;
	my(@value_array) = @_;
	foreach (@value_array) {
		$sum += $_;
	}
	$sum;
}
my (@value_arr) = qw(1 2 3 5 6, 100);
my($hz_sum) = &total(@value_arr);
print "hz_sum = $hz_sum\n";

my($hz_sum) = &total(1..1000);
print "the sum 1~1000 is $hz_sum\n";

#返回数组中大于这些数平均值的数
sub above_average{
	my(@args) = @_;
	my($sum) = &total(@args);
	my($average) = $sum / $#args;
	my(@max_average_element);
	my($element) = 0;
	foreach $element (@args) {
		if ($element >= $average) {
			push(@max_average_element , $element);
		}
	}
	@max_average_element;
}
my (@fred) = &above_average(1..10);
print "fred(1..10) = @fred\n";

my (@fred) = &above_average(100, 1..10);
print "fred(100, 1..10) = @fred\n";

use 5.010;
sub greet {
	state @name_array;
	if ($#name_array < 0) {
		print "Hi @_, You are the first one here.\n";
		push(@name_array, @_);
	} else {
		my($pre_name) = pop(@name_array);
		print "Hi @_, $pre_name is also here.\n";
		push(@name_array, $pre_name);
		push(@name_array, @_);
	}
	@name_array;
}

&greet("Fred");
&greet("Barney");

sub greet_ex {
	state @name_array;
	if ($#name_array < 0) {
		print "Hi @_, You are the first one here.\n";
		push(@name_array, @_);
	} else {
		print "Hi @_, [ @name_array ] is also here.\n";
		push(@name_array, @_);
	}
	@name_array;
}
&greet_ex("Fred");
&greet_ex("Barney");
&greet_ex("Wilma");
&greet_ex("Betty");