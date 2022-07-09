#include <assert.h>
#include "bitops.h"

//TODO: replace with std::bitset
namespace bitops{
	// bitshifting the bits to cut off unwanted ones
	// basically, if you want 3 bits at index 2 of a byte:
	// 12345678 << 2 → 34567800
	// 34567800 >> (8-3) → 00000345
	// magic! and I didn't even copy that from StackOverflow
	inline uint8_t extract_bits(const uint8_t ii,const uint8_t n,const uint8_t v){
		uint8_t tmp = (v << ii);
		return tmp >> (8-n);
		//without the tmp, this would get optimized to (v >> 8-n-ii), which doesn't cut off the bits on the left end
	}
	// getting the needed item & applying the needed bitmask, then shifting the bits to remove the offset
	inline uint8_t read_single(const size_t i,const uint8_t a[]){
		return extract_bits(i%8,1,pgm_read_byte_near(a+i/8));
	}
	// same as above, but also getting the bits that would be overflowing otherwise
	uint8_t read_n(const size_t i,const uint8_t n,const uint8_t a[]){
		//not necessary to check more than one item for overflow, but the assert should be there regardless
		assert(n<=8);
		const uint8_t rbits = extract_bits(i%8,n,pgm_read_byte_near(a+i/8));
		//checking for overflow
		if(n+i%8 > 8){
			const uint8_t overflow_amount = (i+n)%8;
			//making room for missing bits, and extracting them from the next item
			return (rbits << overflow_amount) | extract_bits(0,overflow_amount,pgm_read_byte_near(a[(i/8)+1]));
		}else{
			return rbits;
		}
	}
}