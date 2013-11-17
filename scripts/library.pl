#! /usr/bin/perl -w

#
#   This will create functions of type:
#   int funcI(int arg1, int arg2)
#   {
#       arg1 += rand() % (31 + 3 * I);
#       arg2 += rand() % (131 + 2 * I);
#       arg1 += arg2 / arg1;
#       return arg1 + arg2;
#   }
#
#

if ($#ARGV != 0)
{
    print "Usage $0 num-functions\n";
    exit;
}
open (BIGLIB , ">bigLib.cc") or die "Couldn't open bigLib.cc for writing";

print BIGLIB "#include <cstdio>\n";
print BIGLIB "#include <cstdlib>\n";
print BIGLIB "#include <cassert>\n";

for (my $i = 0; $i < $ARGV[0]; $i++)
{
    $num1 = 31 + $i * 3;
    $num2 = 131 + $i * 2;
    print BIGLIB "int func$i(int arg1, int arg2)\n";
    print BIGLIB "{\n";
    print BIGLIB "  arg1 += rand() % $num1;\n";
    print BIGLIB "  arg2 += rand() % $num2;\n";
    print BIGLIB "  arg1 += arg2 * arg1;\n";
    print BIGLIB "  return ((arg1 + arg2) % 100000000);\n";
    print BIGLIB "}\n\n";
}

# create the libInit() function that will insert pointers to these $ARGV[0] functions
# in an array.
print BIGLIB "typedef int (*Fptr) (int, int); \n"; 
print BIGLIB "void libInit(Fptr* funcTable, int maxIdx)\n";
print BIGLIB "{\n";
print BIGLIB "  assert(maxIdx <= $ARGV[0]);\n";
for (my $i = 0; $i < $ARGV[0]; $i++)
{
    print BIGLIB "  funcTable[$i] = func$i;\n";
}
print BIGLIB "  printf(\"Finished setting up library pointers\\n\");\n";
print BIGLIB "}\n\n";

close(BIGLIB);
print "Compiling the generated library\n";

# Compile bigLib.cc
#`g++ -c bigLib.cc`;
# trying gcc to generate a shared library out of this
`gcc -fPIC -g -c bigLib.cc -o bigLib.o`;
`gcc -shared bigLib.o -o bigLib.so`;

# Print size for generated bigLib.o
my $report = `size bigLib.so`;
print "Generated bigLib.so with following size information:\n";
print "$report";
