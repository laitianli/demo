#!/usr/bin/perl
#
#1.用"%"符号+字符表示一个Hash变量
#2.用胖箭头"=>"建立键与值的对应关系。
#3.用$hash_name{'key'}来访问值
#4.用函数keys获取hash表的键值，用函数values获取hash表的值。
#5.在标量上下文中，函数keys和values都返回键值对个数。

my %hash = (
		aa=>1,
		bb=>2,
		cc=>3,
		dd=>4,
		ff=>5,);
#单个元素插入
$hash{'gg'} = 6; 
$hash{'ee'} = 7; 

my %reverse_hash = reverse %hash;

my @k = keys %hash;
my @v = values %hash;
print "key=@k, value=@v\n";

my $bb = $hash{'bb'};
print "key bb=$bb\n";

my $count = keys %hash;
print "hash count(keys)=$count\n";
$count = values %hash;
print "hash count(values)=$count\n";

print "hash:\n";
#遍历hash表
while (($key, $value) = each %hash) {
#	print "\t$key => $value\n";
	$value += 2;
	$hash{$key} = $value;
}
while (($key, $value) = each %hash) {
	print "\t$key => $value\n";
}

print "reverse hash:\n";
#遍历hash表
while (($key, $value) = each %reverse_hash) {
	print "\t$key => $value\n";
}

#访问环境变量hash: %ENV;
while (($key, $value) = each %ENV) {
	print "\t$key => $value\n";
}

