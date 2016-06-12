#! /usr/bin/perl

print "Hello, world!\n";
use 5.010;
say "say Hello, world!";

$a = 4;
$b = 5;


#/*
# == eq
# != ne
# < lt
# > gt
# <= le
# >= ge
#*/
if($a <= $b) {
	print " \$a < \$b\n";
	print " $a < $b\n";
}

$str1 = aaaa;
$str2 = bbbb;
$str3=$str1.$str2;
$str3.="add string";
print "\$str1.\$str2 = $str3\n";
if($str1 lt $str2){
	print "$str1 < $str2\n";
}
else {
	print "$str1 >= $str2\n";
}

$result = $a < $b;
print "$a < $b \= $result \n";

$count = 0;
while ($count < 10) {
	$count += 1;
	print "count equal $count.\n"
}

print "Please input string: ";
$line = <STDIN>;
#chomp($line=<STDIN>);
if ($line eq "\n") {
	print "that was just a blank line. \n";
} else {
	print "input value is $line\n";
}
