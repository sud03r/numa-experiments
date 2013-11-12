#! /usr/bin/perl -w

if ($#ARGV != 0)
{
    print "Usage $0 num-functions\n";
    exit;
}
open (BIGLIB , ">scriptedDriver.cc") or die "Couldn't open driver.cc for writing";

print BIGLIB "#include <stdlib.h>\n";
print BIGLIB "#include <stdio.h>\n";

print BIGLIB "int main()\n";
print BIGLIB "{\n";
print BIGLIB "  int t = 0;\n";
print BIGLIB "  \n";

for (my $i = 0; $i < $ARGV[0]; $i++)
{
    $funcNum = int(rand(100000));
    $var1 = int(rand(100));
    $var2 = int(rand(100));
    print BIGLIB "  extern int func$funcNum(int, int);\n";
    print BIGLIB "  t &= func$funcNum($var1, $var2);\n";
    print BIGLIB "  \n";
}

print BIGLIB "  return t;\n";
print BIGLIB "}\n\n";

