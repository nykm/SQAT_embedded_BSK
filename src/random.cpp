/*
 * random.c
 *
 *  Created on: Oct 10, 2016
 *      Author: timppa
 */

//
// http://stackoverflow.com/questions/1167253/implementation-of-rand
//

static unsigned int seed_z1 = 12345;
static unsigned int seed_z2 = 12345;
static unsigned int seed_z3 = 12345;
static unsigned int seed_z4 = 12345;

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

unsigned int random_get(void)
{
   unsigned int b;
   b  = ((seed_z1 << 6) ^ seed_z1) >> 13;
   seed_z1 = ((seed_z1 & 4294967294U) << 18) ^ b;
   b  = ((seed_z2 << 2) ^ seed_z2) >> 27;
   seed_z2 = ((seed_z2 & 4294967288U) << 2) ^ b;
   b  = ((seed_z3 << 13) ^ seed_z3) >> 21;
   seed_z3 = ((seed_z3 & 4294967280U) << 7) ^ b;
   b  = ((seed_z4 << 3) ^ seed_z4) >> 12;
   seed_z4 = ((seed_z4 & 4294967168U) << 13) ^ b;
   return (seed_z1 ^ seed_z2 ^ seed_z3 ^ seed_z4);
}
