#! /usr/bin/perl
#
@array = 1..10;
$fred = pop(@array);
print "after pop operation, array value: @array\n";
print "fred=$fred \n";
print pop(@array);
print "after pop operation, array value: @array\n";
push(@array,11);
print "after push(11) operation, array value: @array\n";
print pop(@array);
print "after pop operation, array value: @array\n";


@other = qw(1 3 4 0 2);
push (@array, @other);
print "array: @array\n";
$element=pop(@array);
print "the last element of the array is $element \n";


@str_array = qw(aa bbb ccc ddd eee);
print "str_array size=$#str_array\n";
$m = shift(@str_array);
print "str_array size=$#str_array, m=$m\n";
$n = shift(@str_array);
print "str_array size = $#str_array, n=$n\n";

unshift(@str_array, 'fff');
print "str_array: @str_array\n";
$n = shift(@str_array);
print "str_array size = $#str_array, first element of the str_array is $n\n";


print "str_array=@str_array\n";

@array = qw(aa bb cc dd ee ff gg hh);
$array_size=$#array + 1;
print "array size=$array_size\n";

@remove_array = splice(@array, 3, 2);
print "remove_aray=@remove_array, array=@array\n";
@add_array = qw(ii jj kk ll);
splice(@array, 3, 0, @add_array);
print "after add [ @add_array ] at 3 position, the array is [ @array ]\n";

foreach $arr ( @remove_array) {
	print "arr=$arr\n";
}
$aa = '';
foreach (@array) {
	$aa .= $_;
	$aa .= "\t";
}
print " $aa\n";
