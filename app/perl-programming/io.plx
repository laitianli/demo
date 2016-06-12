#!/usr/bin/perl
#
#

	print "Please input charecter: ";
	#$line = <STDIN>;
	#chomp($line);
	#print "input line is: $line\n";
	print "\n";

	print "Please input charecter: ";
	#while (defined($line=<STDIN>)) {
	#	print "I saw $line";
	#}
	print "\n";

	#从用户指定的位置读取
	#若运行io.plx没有参数，则从标准输入中获取
	#若运行io.plx后有文件名，则从文件中读取
	#文件尾为undef
	#while (defined($line=<>)) {
	#	chomp($line);
	#	print "I saw $line \n";
	#}


	#@ARGV是程序的全局参数
	#@ARGV = qw/text_data text_data2/;
	#while (defined($line=<>)) {
	#	chomp($line);
	#	print "I saw $line \n";
	#}


{
	# "."号为连接两个字符串。
	# "x"表示字符串重复次数后连接起来。
	my(@items) = qw/wilma dino pebbles/;
	printf("the items are:\n".("%10s\n" x @items), @items);
}

{
	# 没有"<" ">" ">>" 表示以可读可写的方式打开文件
	# ">" 打开文件写
	# ">>" 追加的形式打开文件
	# "<" 以读的方式打开文件
	$TEST_FILE_NAME = "io_test_file_name";
	#以追加的方式打开文件
	my($success) = open TMP_FD,'>>',$TEST_FILE_NAME;
	if (!$success) {
		die "open file $TEST_FILE_NAME failed. $!";
	}
	printf "begin copy data from file: @ARGV\n";
	printf TMP_FD "begin copy data from file: @ARGV\n";
	while (defined($line=<>)) {
		chomp($line);
		printf TMP_FD "%s\n", $line;
	}
	printf "copy data from file over\n";
	printf TMP_FD "copy data from file over\n";
	close TMP_FD;
}

{
	# 功能：将一个文件打开，将其内容存放到另一个文件中。
	# 以只读的方式打开/etc/inittab,读取内容，将其写入到临时文件inittab_tmp中，
	# 其中要过滤掉/etc/inittab中注释的内容
	$source_file="/etc/inittab";
	$dest_file="inittab_tmp";
	my($success) = open INITTAB_SRC, '<', $source_file;
	if (!$success) {
		die "open file $source_file failed. $!";
	}
	$success = open INITTAB_DST, '>', $dest_file;
	if (!$success) {
		die "open file $source_file failed. $!";
	}
	while (defined($line=<INITTAB_SRC>)) {
		chomp($line);
#		print "$line\n";
		print INITTAB_DST "$line\n";
	}
	close INITTAB_DST;
	close INITTAB_SRC;
}
