/*
 * random.c
 *
 *  Created on: Oct 10, 2016
 *      Author: timppa
 */

//
// Pseudo Random Number generator code, source: Internet
//
// http://stackoverflow.com/questions/1167253/implementation-of-rand
//
// original seed numbers in the example
//

static unsigned int seed_z1 = 12345;
static unsigned int seed_z2 = 12345;
static unsigned int seed_z3 = 12345;
static unsigned int seed_z4 = 12345;

//
// initializes pseudo random number generator with 4 seed values
//
void random_init(unsigned int s1,
		unsigned int s2,
		unsigned int s3,
		unsigned int s4)
{
	seed_z1=s1;
	seed_z2=s2;
	seed_z3=s3;
	seed_z4=s4;
}

//
// returns next pseudo random number
//
unsigned int random_get(void)
{
   unsigned int temp;
   temp  = ((seed_z1 << 6) ^ seed_z1) >> 13;
   seed_z1 = ((seed_z1 & 4294967294U) << 18) ^ temp;
   temp  = ((seed_z2 << 2) ^ seed_z2) >> 27;
   seed_z2 = ((seed_z2 & 4294967288U) << 2) ^ temp;
   temp  = ((seed_z3 << 13) ^ seed_z3) >> 21;
   seed_z3 = ((seed_z3 & 4294967280U) << 7) ^ temp;
   temp  = ((seed_z4 << 3) ^ seed_z4) >> 12;
   seed_z4 = ((seed_z4 & 4294967168U) << 13) ^ temp;
   return (seed_z1 ^ seed_z2 ^ seed_z3 ^ seed_z4);
}
