#include<stdio.h>
#include <stdint.h>

uint32_t L32 (uint32_t x)
{
  int a = 11, b0 = 5, b1 = 9, b2 = 12;
  uint32_t y=0;
  for (int i=0; i<32; i++)
    y ^= (((x>>((a*i+b0)%32)) ^ (x>>((a*i+b1)%32)) ^ (x>>((a*i+b2)%32))) & 0x1) << i;
  return y;
}

uint32_t L_32 (uint32_t x)
{
  int a = 11, b0 = 1, b1 = 26, b2 = 30;
  uint32_t y=0;
  for (int i=0; i<32; i++)
    y ^= (((x>>((a*i+b0)%32)) ^ (x>>((a*i+b1)%32)) ^ (x>>((a*i+b2)%32))) & 0x1) << i;
  return y;
}

uint64_t L64 (uint64_t x)
{
  int a = 3, b0 = 1, b1 = 26, b2 = 50;
  uint64_t y=0;
  for (int i=0; i<64; i++)
    y ^= (((x>>((a*i+b0)%64)) ^ (x>>((a*i+b1)%64)) ^ (x>>((a*i+b2)%64))) & 0x1) << i;
  return y;
}

/*void L128 (uint64_t* x, uint64_t* y)
{
  int a = 17, b0 = 7, b1 = 11, b2 = 14;
  uint64_t temp;
  for (int i=0; i<64; i++)
  {
    int p=(a*i+b0)%128, q=(a*i+b1)%128, r=(a*i+b2)%128;
    temp = 0;
    
    if (p <= 63)
      temp = x[0]>>p;
    else
      temp = x[1]>>(p-64);
      
    if (q <= 63)
      temp ^= x[0]>>q;
    else
      temp ^= x[1]>>(q-64);
      
    if (r <= 63)
      temp ^= x[0]>>r;
    else
      temp ^= x[1]>>(r-64);
      
    y[0] ^= (temp & 0x1) << i; 
  }
  
  for (int i=64; i<128; i++)
  {
    int p=(a*i+b0)%128, q=(a*i+b1)%128, r=(a*i+b2)%128;
    temp = 0;
    
    if (p <= 63)
      temp = x[0]>>p;
    else
      temp = x[1]>>(p-64);
      
    if (q <= 63)
      temp ^= x[0]>>q;
    else
      temp ^= x[1]>>(q-64);
      
    if (r <= 63)
      temp ^= x[0]>>r;
    else
      temp ^= x[1]>>(r-64);
      
    y[1] ^= (temp & 0x1) << (i-64);
  }
}*/

void L128 (uint64_t* x, uint64_t* y)
{
  int a = 17, b0 = 7, b1 = 11, b2 = 14;
  int p[3]={0};
  uint64_t temp1;
  
  for(int i=0; i<128; i++)
  {
    p[0] = (a*i+b0)%128;
    p[1] = (a*i+b1)%128;
    p[2] = (a*i+b2)%128;
    
    temp1 = 0;
    for (int j=0; j<3; j++)
    {
      uint64_t temp2 = (p[j] < 64) ? x[0] : x[1];
      int shift = (p[j] < 64) ? p[j] : p[j]-64;
      temp1 ^= (temp2 >> shift) & 0x1;
    }
    
    if (i<64)
      y[0] ^= temp1 << i;
    else
      y[1] ^= temp1 << (i-64);
  }
}

/*uint32_t RightRot32 (uint32_t x, int n)
{
  return (x >> n) | (x << (32-n));
}

uint32_t chi32 (uint32_t x)
{
  uint32_t y = x ^ ((RightRot32(x,1) ^ 0xffffffff) & RightRot32(x,2));
  return y;
}

uint64_t RightRot64 (uint64_t x, int n)
{
  return (x >> n) | (x << (64-n));
}

uint64_t chi64 (uint64_t x)
{
  uint64_t y = x ^ ((RightRot64(x,1) ^ 0xffffffff) & RightRot64(x,2));
  return y;
}*/

uint32_t ChiChi32 (uint32_t x)
{
  uint32_t y = 0;
  int i, m=16;
  
  for (i=0; i<m-3; i++)
    y ^= (((x>>i) ^ (~(x>>(i+1)) & (x>>(i+2)))) & 0x1) << i;
  
  y ^= (((x>>m) ^ (~(x>>(m-2)) & x)) & 0x1) << m-3;
  y ^= (((x>>(m-1)) ^ (~x & (x >> 1))) & 0x1) << m-2;
  y ^= ((~(x>>(m-3)) ^ (~(x >> m) & (~(x>>(m+1))))) & 0x1) << m-1;
  y ^= (((x>>(m-2)) ^ (~(x>>(m+1)) & (x>>(m+2)))) & 0x1) << m;
  
  for (i=m+1; i<(2*m-2); i++)
    y ^= (((x>>i) ^ (~(x>>(i+1)) & (x>>(i+2)))) & 0x1) << i;
    
  y ^= (((x>>(2*m-2)) ^ (~(x>>(2*m-1)) & (x>>(m-1)))) & 0x1) << (2*m-2);
  y ^= (((x>>(2*m-1)) ^ (~(x>>(m-1)) & (x>>m))) & 0x1) << (2*m-1);
  
  return y;
}

uint64_t ChiChi64 (uint64_t x)
{
  uint64_t y = 0;
  int i, m=32;
  
  for (i=0; i<m-3; i++)
    y ^= (((x>>i) ^ (~(x>>(i+1)) & (x>>(i+2)))) & 0x1) << i;
  
  y ^= (((x>>m) ^ (~(x>>(m-2)) & x)) & 0x1) << m-3;
  y ^= (((x>>(m-1)) ^ (~x & (x >> 1))) & 0x1) << m-2;
  y ^= ((~(x>>(m-3)) ^ (~(x >> m) & (~(x>>(m+1))))) & 0x1) << m-1;
  y ^= (((x>>(m-2)) ^ (~(x>>(m+1)) & (x>>(m+2)))) & 0x1) << m;
  
  for (i=m+1; i<(2*m-2); i++)
    y ^= (((x>>i) ^ (~(x>>(i+1)) & (x>>(i+2)))) & 0x1) << i;
    
  y ^= (((x>>(2*m-2)) ^ (~(x>>(2*m-1)) & (x>>(m-1)))) & 0x1) << (2*m-2);
  y ^= (((x>>(2*m-1)) ^ (~(x>>(m-1)) & (x>>m))) & 0x1) << (2*m-1);
  
  return y;
}

void ChiChi128 (uint64_t* x, uint64_t* y) //lsb in x[0], msb in x[1]
{
  int i, m=64;
  
  for (i=0; i<m-3; i++)
    y[0] ^= (((x[0]>>i) ^ (~(x[0]>>(i+1)) & (x[0]>>(i+2)))) & 0x1) << i;
    
  y[0] ^= ((x[1] ^ (~(x[0]>>(m-2)) & x[0])) & 0x1) << m-3;
  y[0] ^= (((x[0]>>(m-1)) ^ (~x[0] & (x[0] >> 1))) & 0x1) << m-2;
  y[0] ^= ((~(x[0]>>(m-3)) ^ (~x[1] & (~(x[1]>>1)))) & 0x1) << m-1;
  y[1] ^= (((x[0]>>(m-2)) ^ (~(x[1]>>1) & (x[1]>>2)))) & 0x1;
  
  for (i=1; i<m-2; i++)
    y[1] ^= (((x[1]>>i) ^ (~(x[1]>>(i+1)) & (x[1]>>(i+2)))) & 0x1) << i;
    
  y[1] ^= (((x[1]>>(m-2)) ^ (~(x[1]>>(m-1)) & (x[0]>>(m-1)))) & 0x1) << (m-2);
  y[1] ^= (((x[1]>>(m-1)) ^ (~(x[0]>>(m-1)) & x[1])) & 0x1) << (m-1);
}

void rndcnst (uint32_t* rc)
{
  for (int i=0; i<8; i++)
   rc[i] = i ^ (1 << (i+4));
}

uint32_t dec (uint64_t* K, uint64_t T, uint32_t C, uint16_t G_Tag, int Tau, uint32_t* rc)
{
  uint32_t C1 = C ^ (K[1] & 0xffffffff);
  uint32_t C2 = C ^ (K[1] >> 32);
  T ^= K[0];
  uint32_t temp32;
  uint32_t temp_32;
  uint64_t temp64;
  uint64_t temp128[2];
  
  for (int i=0; i<7; i++)
  {
    K[1] = (K[1] & 0xffffffff) ^ ((K[1] >> 32) ^ rc[i])<<32;
    
    temp32 = temp_32 = temp64 = temp128[0] = temp128[1] = 0;
    
    temp32 = ChiChi32(C1);
    temp_32 = ChiChi32(C2);
    temp64 = ChiChi64(T);
    ChiChi128(K, temp128);
    
    C1 = C2 = 0;
    T=K[0]=K[1]=0;
    
    C1 = L32(temp32);
    C2 = L_32(temp_32);
    T = L64(temp64);
    L128(temp128, K);
    
    C1 ^= T & 0xffffffff;
    C2 ^= T >> 32;
    T ^= K[0];
    
    /*printf("%d\n", i);
    printf("X = 0x%08X\n", C);
    printf("T = 0x%016lX\n", T);
    printf("K = 0x%016lX", K[1]);
    printf("0x%016lX\n\n\n", K[0]);*/
  }
  
  temp32 = temp_32 = temp64 = 0;
  
  temp32 = ChiChi32(C1);
  temp_32 = ChiChi32(C2);
  temp64 = L64(T);

  uint16_t Tag = (temp_32 ^ (temp64 >> 32)) >> (32-Tau);
    printf("0x%08X\n", Tag);
   
  if (Tag == G_Tag)
    return (temp32 ^ (temp64 & 0xffffffff));
  else 
    return 0;
}

int main()
{
  uint64_t K[2] = {0x7766554433221100, 0xFEDCBA9876543210};
  uint64_t T =  0x0011223344556677;
  uint32_t C =  0x01234567;
  uint32_t rc[8];
  rndcnst (rc);
  int Tau = 8;
  uint16_t G_Tag = 0x0f;
  uint32_t P = dec (K, T, C, G_Tag, Tau, rc);
  
  printf("0x%08X\n", P);
    
  return 0;
}
