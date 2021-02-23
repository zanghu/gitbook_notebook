## Hash函数现状概览（截至2012）

本文来自外文翻译

**参考资料**

英文原文：[State of the hash functions, 2012](http://blog.reverberate.org/2012/01/state-of-hash-functions-2012.html)

中文翻译：[Hash 函数概览](https://www.oschina.net/translate/state-of-hash-functions)

### 哈希函数现状，2012

**2012年01月29日**

最先进的非加密散列函数在过去几年中得到了快速推广。当我这周搜索的时候，我很高兴的看到新的尖端散列函数已经发布即使上次我进行这个方面的搜索是6个月到1年前的事情了。

非加密散列函数将字符串作为输入，通过计算输出一个整数。理想的散列函数的一个特性是输出非常均匀分布在可能的输出域，特别是当输入非常相似的时候。不同于加密散列函数，这些函数不是为防止攻击者找出碰撞而设计的。加密散列函数有这个特性但是要慢的多： [SHA-1大约为0.09 bytes/cycle](http://www.cryptopp.com/benchmarks.html)，而最新的非加密散列函数的速度大约为3 bytes/cycle。所以在不考虑抵御攻击的成本下，非加密散列大约要快33倍。非加密散列函数用的最多的地方是hash table。
 
一个有趣的题外话，[现在Lua社区上有个关于针对Lua的散列函数理论上可以被攻击使得hash table最坏情况下的查找复杂度降低到O(n) 的事实我们应该做些什么（如果可以的话）的争论](http://thread.gmane.org/gmane.comp.lang.lua.general/87491)，如果攻击者引导你输入你放入lua hash table的东西，那么他可以通过DoS攻击你。Lua的作者怀疑这种攻击实施的可行性（以及它的代价是否比其他DoS攻击更小），但是他还是打算在将要使用的散列函数的开始时候生成随机种子。这是一个有趣的加密替代函数，能够给你与加密散列函数相同的耐碰撞能力（假设你有能够给你真正随机位的信息源），但这将以非重复性输出为代价。

由于非加密散列函数有很多种选择，并且可供选择的数目不断扩大，我想我应该总结以下我对这个领域的了解。

***

### Bob Jenkins' Functions
[Bob Jenkins](http://burtleburtle.net/bob/)已经在散列函数领域工作了将近15年。在1997年他在《 Dr. Dobbs Journal》杂志上发表了一片关于散列函数的文章[《A hash function for hash Table lookup》](http://www.burtleburtle.net/bob/hash/doobs.html)，这篇文章自从发表以后现在网上有更多的扩展内容。这篇文章中，Bob广泛收录了很多已有的散列函数，这其中也包括了他自己所谓的“lookup2”。随后在2006年，Bob发布了[lookup3](http://burtleburtle.net/bob/c/lookup3.c)，由于它即快速（Bob自称，0.5 bytes/cycle）又无严重缺陷，在这篇文章中我把它认为是第一个“现代”散列函数。

更多有关Bob's散列函数的内容请参阅维基百科：[Jenkins hash function](http://en.wikipedia.org/wiki/Jenkins_hash_function).

***

### 第二代: MurmurHash
Austin Appleby在2008年发布了一个新的散列函数——MurmurHash。其最新版本大约是lookup3速度的2倍（大约为1 byte/cycle），它有32位和64位两个版本。32位版本只使用32位数学函数并给出一个32位的哈希值，而64位版本使用了64位的数学函数，并给出64位哈希值。根据Austin的分析，MurmurHash具有优异的性能，虽然Bob Jenkins 在《Dr. Dobbs article》杂志上声称“我预测[MurmurHash ]比起lookup3要弱，但是我不知道具体值，因为我还没测试过它”。MurmurHash能够迅速走红得益于其出色的速度和统计特性。

***

### 第三代: CityHash 和 SpookyHash
2011年，发布了两个散列函数，相对于[MurmurHash](https://sites.google.com/site/murmurhash/) ，它们都进行了改善，这主要应归功于更高的指令级并行机制。Google发布了CityHash（由Geoff Pike 和Jyrki Alakuijala编写），Bob Jenkins发布了他自己的一个新散列函数SpookyHash（这样命名是因为它是在万圣节发布的）。它们都拥有2倍于MurmurHash的速度，但他们都只使用了64位数学函数而没有32位版本，并且CityHash的速度取决于CRC32 指令，目前为SSE 4.2（Intel Nehalem及以后版本）。SpookyHash给出128位输出，而CityHash有64位，128位以及256位的几个变种。

***

### 哪一个散列函数最好/最快？
从我所了解的情况来看，这篇文章中所提到的所有散列函数从统计学角度来看已经足够好。需要考虑的一个因素是[CityHash](http://code.google.com/p/cityhash/)/[SpookyHash](http://burtleburtle.net/bob/hash/spooky.html)的输出超过了64位，但是对于一个32位的hash table来说这输出太多了。其他应用可能会用到128或256位输出。

如果你用的是32位机器，MurmurHash看起来是最明显的赢家，因为它是唯一一个快于lookup3的32位原生版本。32位机器很可能可以编译并运行City和Spooky，但我预计它们的运行速度和在64位机器上运行的速度比起来要慢的多，毕竟32位机器需要模拟64位的数学运算。

在64位机器上，由于没有更深层次的基准，也很难说哪种算法是最好的。比起City我更倾向于Spooky，因为City的运行速度需要依赖于CRC32指令，毕竟这种环境并不是任何机器上都有的。

***

另一个需要考虑的是对齐和非对齐的访问。Murmur散列（不像City或者Spooky）是一个仅能进行对齐读取的变种，因为在很多架构上非对齐的读取会崩溃或者返回错误的数据（非对齐的读取操作在C中是未定义的行为）。City和Spooky都强调使用memcpy()把输入数据复制到对齐的存储结构中；Spooky使用一次memcpy()操作一个块（如果ALLOW_UNALIGNED_READS未定义），City使用一次memcpy()操作一个整型！在可以处理非对称读取的机器上（像x86和x86-64），memcpy将被优化，但我在我的小ARM上做了一个测试，发现如下：

```c
#include <stdint.h>
#include <string.h>
int32_t read32_unaligned(const void *buf) {
  int32_t ret;
  memcpy(&ret, buf, 4);
  return ret;
}
```

编译这段低效的代码（在x86上是一个单独的操作）：

```x86asm
   0: b500       push {lr}
   2: 2204       movs r2, #4
   4: b083       sub sp, #12
   6: 4601       mov r1, r0
   8: eb0d 0002  add.w r0, sp, r2
   c: f7ff fffe  bl 0 
  10: 9801       ldr r0, [sp, #4]
  12: b003       add sp, #12
  14: bd00       pop {pc}
```

结论是，如果你需要32位或者仅仅是对齐读取的话，Murmur散列看起来依旧是最好的选择。City散列和Spooky散列在x86-64上看起来更快，但我更倾向于认为它们是特定用于那个架构的，因为我不知道是否有其他既是64位又允许非对其读取的架构。