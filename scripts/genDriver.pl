#! /usr/bin/perl -w

if ($#ARGV != 0)
{
    print "Usage $0 num-functions\n";
    exit;
}
open (DRIVER , ">driver.cc") or die "Couldn't open driver.cc for writing";

print DRIVER "#include <stdlib.h>\n";
print DRIVER "#include <stdio.h>\n";

print DRIVER "int main()\n";
print DRIVER "{\n";
print DRIVER "  int t = 0;\n";
print DRIVER "  \n";

for (my $i = 0; $i < $ARGV[0]; $i++)
{
    $funcNum = int(rand(100000));
    $var1 = int(rand(100));
    $var2 = int(rand(100));
    print DRIVER "  extern int func$funcNum(int, int);\n";
    print DRIVER "  t &= func$funcNum($var1, $var2);\n";
    print DRIVER "  \n";
}

print DRIVER "  printf(\"driver.cc done!\\n\");\n";
print DRIVER "  return t;\n";
print DRIVER "}\n\n";
close(DRIVER);

