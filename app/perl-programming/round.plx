#! /usr/bin/perl

print "please input radius: ";
$R=<STDIN>;
chomp($R);
$PI=3.1415926;
$round=2*$PI*$R;

print "radius=$R round is $round\n";
