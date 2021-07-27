/*
 * A generic kernel FIFO implementation
 *
 * Copyright (C) 2013 Stefani Seibold <stefani@seibold.net>
 */

#ifndef _LINUX_KFIFO_H
#define _LINUX_KFIFO_H

/*
 * How to porting drivers to the new generic FIFO API:
 *
 * - Modify the declaration of the "struct kfifo *" object into a
 *   in-place "struct kfifo" object
 * - Init the in-place object with kfifo_alloc() or kfifo_init()
 *   Note: The address of the in-place "struct kfifo" object must be
 *   passed as the first argument to this functions
 * - Replace the use of __kfifo_put into kfifo_in and __kfifo_get
 *   into kfifo_out
 * - Replace the use of kfifo_put into kfifo_in_spinlocked and kfifo_get
 *   into kfifo_out_spinlocked
 *   Note: the spinlock pointer formerly passed to kfifo_init/kfifo_alloc
 *   must be passed now to the kfifo_in_spinlocked and kfifo_out_spinlocked
 *   as the last parameter
 * - The formerly __kfifo_* functions are renamed into kfifo_*
 */

/*
 * Note about locking: There is no locking required until only one reader
 * and one writer is using the fifo and no kfifo_reset() will be called.
 * kfifo_reset_out() can be safely used, until it will be only called
 * in the reader thread.
 * For multiple writer and one reader there is only a need to lock the writer.
 * And vice versa for only one writer and multiple reader there is only a need
 * to lock the reader.
 */

// #include <linux/kernel.h>
// #include <linux/spinlock.h>
// #include <linux/stddef.h>
// #include <linux/scatterlist.h>

// liigo 20190919
#include <stdlib.h>
#include <errno.h>
#define __must_check
#define ARRAY_SIZE(ary) (sizeof((ary))/sizeof(*(ary)))
#ifdef __GNUC__
	#define typeof __typeof__
	#define smp_wmb __sync_synchronize
#endif
// end

struct __kfifo {
	unsigned int	in;
	unsigned int	out;
	unsigned int	mask;
	unsigned int	esize;
	void		*data;
};

#define __STRUCT_KFIFO_COMMON(datatype, recsize, ptrtype) \
	union { \
		struct __kfifo	kfifo; \
		datatype	*type; \
		const datatype	*const_type; \
		char		(*rectype)[recsize]; \
		ptrtype		*ptr; \
		ptrtype const	*ptr_const; \
	}

#define __STRUCT_KFIFO(type, size, recsize, ptrtype) \
{ \
	__STRUCT_KFIFO_COMMON(type, recsize, ptrtype); \
	type		buf[((size < 2) || (size & (size - 1))) ? -1 : size]; \
}

#define STRUCT_KFIFO(type, size) \
	struct __STRUCT_KFIFO(type, size, 0, type)

#define __STRUCT_KFIFO_PTR(type, recsize, ptrtype) \
{ \
	__STRUCT_KFIFO_COMMON(type, recsize, ptrtype); \
	type		buf[0]; \
}

#define STRUCT_KFIFO_PTR(type) \
	struct __STRUCT_KFIFO_PTR(type, 0, type)

/*
 * define compatibility "struct kfifo" for dynamic allocated fifos
 */
struct kfifo __STRUCT_KFIFO_PTR(unsigned char, 0, void);

#define STRUCT_KFIFO_REC_1(size) \
	struct __STRUCT_KFIFO(unsigned char, size, 1, void)

#define STRUCT_KFIFO_REC_2(size) \
	struct __STRUCT_KFIFO(unsigned char, size, 2, void)

/*
 * define kfifo_rec types
 */
struct kfifo_rec_ptr_1 __STRUCT_KFIFO_PTR(unsigned char, 1, void);
struct kfifo_rec_ptr_2 __STRUCT_KFIFO_PTR(unsigned char, 2, void);

/*
 * helper macro to distinguish between real in place fifo where the fifo
 * array is a part of the structure and the fifo type where the array is
 * outside of the fifo structure.
 */
#define	__is_kfifo_ptr(fifo) \
	(sizeof(*fifo) == sizeof(STRUCT_KFIFO_PTR(typeof(*(fifo)->type))))

/**
 * DECLARE_KFIFO_PTR - macro to declare a fifo pointer object
 * @fifo: name of the declared fifo
 * @type: type of the fifo elements
 */
#define DECLARE_KFIFO_PTR(fifo, type)	STRUCT_KFIFO_PTR(type) fifo

/**

 公会银行

 尘泥沼泽

 20到60级之间升级所需的经验值降低了。另外，30到60级之间完成任务获得的经验值提高了。

 综合
 - 《魔兽世界：燃烧的远征》的最新升级补丁加入了对当今最流行的多核处理器(比如Intel(r)的Core(tm)2 Duo处理器系列)的支持，提高了使用多核处理器时的运行速度。
 - 祖阿曼：祖阿曼是一个位于幽魂之地的70级10人户外副本。玩家将遭遇包括森林巨魔酋长祖尔金在内的六个全新首领。
 - 公会银行：公会银行是一个属于整个公会的共享储存室，可以通过一个新的公会仓库界面来使用。公会银行允许具备适当权限的玩家储存物品和金币，以供公会其他成员使用。
 - 经验值：减少了20级至60级之间升级每级所需的经验值，并增加了30级至60级任务给予的经验。
 - 精准：加入了一个新的、与等级系统关联的属性——精准和精准等级。精准等级转换成精准的比例与以前武器技能的转换相同。每一点精准可以使你的攻击被躲闪或招架的几率降低0.25%。
 - 降低治疗效果现在会对所有吸取生命法术和技能起作用(比如说：致死打击会降低通过吸取生命获得的生命值)。
 - 治疗：几乎所有提供额外治疗效果的物品和附魔，现在同样会提供少量(大约1/3)的额外法术伤害。但也有一些物品和附魔不受这个效果影响，比如说随机属性的物品和祖尔格拉布的附魔，除这些例子以外对所有其它物品都有作用。
 - 击杀效果：通过击杀敌人触发效果的物品和技能不再可以通过杀死自己的宠物而触发。
 - 武器技能：修改了所有提供武器技能的物品和技能。大多数情况下，它们被转换成精准或者精准等级。远程攻击并不受到精准加成，因此远程武器技能通常情况下被替换成爆击奖励或者命中奖励。在某些情况下，天赋被修改成其它效果来避免玩家获得过多的精准。
 - 在荆棘谷北面的反抗军营地增加了一条新的飞行航线。
 - 飞行单位可以看到地面单位，反之，地面单位也可以看到在任何高度的飞行单位。
 - 玩家不再可以通过跳跃获得活动的AOE范围奖励。
 - 客户端法术施法请求即使在你已经施放了一个其它法术的时候也会被传送到服务器端。这样就可以不用在宏中使用/stopcasting命令弥补网络带来的延迟了。
 - 声望折扣：所有和声望相关联的商人现在都会给声望中立以上的玩家一定的折扣。
 - 友善：5% 折扣
 - 尊敬：10% 折扣
 - 崇敬：15% 折扣
 - 崇拜：20% 折扣
 - 宠物在近身战斗时会尝试绕到它们的目标背后进行攻击。
 - 与孢子人声望达到崇拜之后可以购买小孢子蝠宠物。
 - 艾萨拉的蓝龙会掉落蓝龙宝宝宠物。
 - 如果你处于坐下的状态，并且试图使用需要站立才能使用的技能，你会自动站起来并使用该技能。你可以使用/console autoStand 0命令来禁止这个功能。
 种族特长
 - 矮人：枪械专精从增加武器技能修改为提高枪械爆击几率1%。
 - 侏儒：逃命专家的冷却时间增加到1分45秒。
 - 巨魔：弓和投掷专精从增加武器技能修改为提高弓和投掷武器的爆击几率1%。
 - 人类：剑技能现在可以提高剑和双手剑的爆击几率1%。
 - 人类：锤技能现在可以提高锤和双手锤的爆击几率1%。
 - 兽人：斧技能现在可以提高斧和双手斧的爆击几率1%。
 PvP
 - 竞技场
 - 每个战场组中的一定比例的顶级战队将在第二赛季结束后获得一个新的头衔“残酷角斗士”。
 - 战场
 - 增加了针对某个随机战场的日常任务。在你所属阵营的主城的阵营军官附近寻找联盟准将和部落战争使者来接到此类任务。
 - 在战场中扔掉了军旗将获得一个3秒内无法再次拾取军旗的减益效果。
 - 在战场中得到一个恢复类效果时，变形状态下的德鲁伊可以正确地恢复法力值。
 - 玩家可以在战场开始前不消耗法力地施放任何法术。
 - 玩家在战场中被灵魂医师复活后，将得到几秒不消耗法力的施法时间。
 - 奥特兰克山谷
 - 将领/统帅不再履行报告摧毁敌方哨塔/碉堡的职责。然而，摧毁一座哨塔/碉堡仍然会消灭一个对方的将领/统帅。
 - 所有的将领/统帅都和他们各自的将军相互关联，不能再单独拉出。
 - 增加了占领哨塔/碉堡所给予的荣誉。
 - 所有的中尉和指挥官都离开了奥特兰克山谷，寻找其它的战斗机会。
 - 在战场中，除非一方没有墓地，玩家将不会再被传送到他们的起始隧道。
 - 很多雷矛和霜狼基地内的NPC将不再是精英。
 - 现在在奥特兰克山谷中，只能通过在战斗中摧毁敌方的哨塔/碉堡和杀死对方的首领获得奖励荣誉。在战斗结束时，同样会通过未被摧毁的哨塔/碉堡、幸存的首领和获胜获得奖励荣誉。奖励荣誉总数应该和以前的总数相同。
 - 部落和联盟现在在奥特兰克山谷中有一定的援军值。玩家死亡、哨塔/碉堡被摧毁、加尔范上尉或者巴琳达•斯通赫尔斯被杀死时都会减少一定量的援军值。如果一方队伍的援军值降到了0，对方队伍将获胜。
 - 哨塔/碉堡和墓地的占领时间从5分钟改为4分钟。
 - 风暴之眼
 - 在风暴之眼里夺取军旗奖励的点数根据队伍占领的基地数量决定。
 - 战歌峡谷，阿拉希盆地，风暴之眼
 - 战斗结束后会得到额外的奖励荣誉。
 德鲁伊
 - 树皮术：在受到飓风术影响的情况下不再能施放这个法术。
 - 现在可以在生命之树形态下使用消毒术和驱毒术。
 - 消毒术、驱毒术和解除诅咒的范围增加到40码。
 - 纠缠根须：多个德鲁伊无法再对同一个目标施放纠缠根须，如果其他德鲁伊对该目标施放纠缠根须，那么该效果将正确覆盖掉前面那名德鲁伊的纠缠根须效果。
 - 野性攻击强度：提高猎豹、熊、巨熊和枭兽攻击强度的物品都进行了重新评估。大部分物品增加了攻击强度。这个改变修正了野性武器伤害与其他职业相比之下并不保持相同增长率的问题。
 - 自然之力：这个技能不再自动解除施法区域内受害人的潜行状态。在附近潜行的角色如果太靠近召唤出的树人仍然会被攻击到。
 - 野性之心：这个天赋不再给猎豹形态提供4/8/12/16/20%的力量奖励，而改为提供2/4/6/8/10%攻击强度。
 - 休眠：如果在多个德鲁伊互相覆盖休眠时，该法术现在将始终会被正确去除。
 - 强烈(恢复系天赋) 法力回复速度增加到10/20/30%。
 - 原始狂怒：这个天赋现在即使在德鲁伊死亡的情况下学习也可正确学到。
 - 复生的冷却时间从30分钟减少到20分钟。
 - 现在可以在枭兽形态下使用解除诅咒。
 - 变形法术：有些法术会引起超出设计本意的额外仇恨值，这些额外的仇恨值已被去除。
 - 安抚动物：这个法术现在会正确地消耗自然迅捷。
 - 宁静现在可以得到额外的法伤和治疗效果加成。
 - 天然完美(恢复系天赋)：现在也会在受到爆击之后获得天然完美效果，在6秒内受到的所有伤害降低1/3/5%，可叠加3次。
 - 如果你在变形状态下试图使用只能在人形状态下使用的技能，你会自动取消变形形态，并使用该技能。这就意味着你可以直接从一种形态转变为另一种形态。如果你没有足够的法力值，那么你将看到一条错误信息，并保持在原有的形态。你可以通过输入命令/console autoUnshift 0来禁用这一功能。
 - 挑战咆哮：该技能命中目标的几率现在会受到命中等级的影响。
 - 纠缠根须：当一个伤害更低的纠缠根须效果覆盖原有的纠缠根须效果时，该技能不再被覆盖。
 - 野性冲锋：该技能的打断和昏迷效果可以分别被抵抗了。
 - 低吼：该技能命中目标的几率现在会受到命中等级的影响。
 - 天然完美：该天赋在德鲁伊坐下时也可以触发了。
 - 生命之树：当德鲁伊不在生命之树形态下时，不再会获得治疗加成的效果。
 猎人
 - 瞄准射击(射击天赋)现在可以使目标受到的治疗效果降低50%，持续10秒。施放时间缩短0.5秒。
 - 奥术射击(等级6或以上)现在除正常伤害外还可以驱散一个魔法效果。
 - 爆炸陷阱：该陷阱的初始伤害现在受到你10%的远程攻击强度加成。
 - 献祭陷阱：该陷阱的总计伤害现在受到你10%的远程攻击强度加成。
 - 误导：如果多个猎人互相覆盖其他猎人的标记，该法术现在会始终被正确去除。
 - 准备就绪：该天赋现在可以结束杀戮命令的冷却时间。
 - 毒蛇钉刺、献祭陷阱和爆炸陷阱都得到了基于远程攻击强度的额外伤害。
 - 毒蛇钉刺：该钉刺的总计伤害现在受到你10%的远程攻击强度加成。
 - 稳固射击：提示框阐明了基础武器伤害被使用到伤害计算中。
 - 施放强击光环 (射击天赋)不再需要法力而且会一直存在直到你取消它。
 - 翼龙钉刺(生存天赋)现在是瞬发法术，但在PvP中最多只有10秒的持续时间。
 - 破甲虚弱：该天赋所触发的效果不再消耗误导的使用次数。
 - 误导：如果一个猎人试图对一个已经受到误导效果影响的目标使用误导，则该法术会失败(因为已有一个更强大的效果在发挥作用)，不会浪费冷却时间或者法力值。
 - 翼龙钉刺：该技能不再被低等级的翼龙钉刺覆盖。
 法师
 - 降低了奥术智慧和奥术光辉的法力消耗。
 - 奥术冥想 (奥术天赋)法力回复速度增加到 10/20/30%。
 - 奥术飞弹：玩家将不再在对闪避状态下的目标施放奥术飞弹时得到奇怪的错误信息，该法术将直接失效。
 - 侦测魔法被移除。现在所有玩家都可以一直看到他们目标身上的增益效果。
 - 唤醒现在每2秒回复全部法力的15%法力，取代了之前以精神属性为基础的回复。
 - 防护火焰结界和防护冰霜结界现在可以获得法伤的加成。等级5和等级6的基础吸收值被降低。
 - 寒冰护体现在可以获得法伤加成。等级5和等级6的基础吸收值降低。
 - 强化火球术：去除了由这个天赋引起的伤害降低。
 - 强化寒冰箭：去除了由这个天赋引起的伤害降低。
 - 变形术：不再可能在变形一个玩家时该玩家仍然保持骑乘状态。另外，如果多个法师互相覆盖变形术，变形术效果将会始终被正确去除。
 - 传送门法术：在战场内不再能施放传送门法术。
 - 解除诅咒的范围增加到40码。
 - (新法术) 70级时可以在训练师那里学到新的法术“餐桌”。
 - 法术窃取：无法再通过这个法术获得两个自律效果。
 - 点燃：该天赋不再被熔岩护甲触发。
 圣骑士
 - 光明祝福：低等级的圣光闪现和圣光术在使用该祝福的情况下会得到正确的效果削减。
 - 清洁术和纯净术的范围增加到40码。
 - 十字军打击(惩戒天赋)的冷却时间从10秒降低到6秒。
 - 神恩术：如果该技能在一次治疗爆击后立即施放，将不再被错误地移除。
 - 降低了驱邪术的法力消耗。
 - 降低了愤怒之锤的法力消耗。
 - 降低了神圣愤怒的法力消耗。
 - 狂热(惩戒天赋)：现在也会令所有行为造成的威胁值降低6/12/18/24/30%。
 - 精确(防护天赋)：现在也会提高法术命中率。
 - 强化十字军圣印(惩戒天赋)受到基础法伤加成。这个天赋现在受到神圣十字军天赋的加成。
 - 圣光审判：战斗记录现在会显示当圣光审判取代光明圣印时，从等级5的该技能上获得的法力值。
 - 复仇圣印：当这个效果在目标身上完全叠加时，该技能所造成的伤害过低，这个错误已被修正。
 - 精神协调：该技能现在可以从吸血鬼的拥抱正确地获得法力值了。
 - 正义追击(惩戒天赋)现在有3个等级，并增加5/10/15%的移动速度，同样降低你被法术击中的几率1/2/3%。
 - 神圣十字军(惩戒天赋)改名为净化圣印，现在提高你的法术和近战爆击几率1/2/3%，并降低你的圣印被驱散的几率33/66/100%。
 - 复仇(惩戒天赋)的持续时间从15秒增加到30秒。
 - 辩护(惩戒天赋)的频率和持续时间增加，而且现在降低5/10/15%的所有属性，而不只是力量和敏捷。
 - 武器专家 (防护天赋)被改名为战斗精准，现在增加1/2/3/4/5的精准值，并提高2/4/6/8/10%的耐力总值。
 - 智慧审判：法力回复效果将不再反复触发智慧审判。
 - 当你投入了3点天赋在净化圣印上之后，圣印将无法被驱散。
 - 正义防御：该技能命中目标的几率会受到命中等级影响了。
 牧师
 - 责罚(新技能)是一个矮人和德莱尼牧师在20级可以获得的新技能。责罚造成神圣伤害并让目标瘫痪2秒。
 - 治疗之环：这个法术的基础治疗量由于受到治疗效果的加成而降低。拥有1338或以上治疗效果的角色将看到他们的治疗之环比以前版本治疗的更多。而治疗效果小于1338的角色会看到他们的治疗之环比以前治疗的少。
 - 治愈疾病和驱除疾病的范围增加到40码。
 - 艾露恩的赐福 (暗夜精灵)的效果改变为使近战和远程攻击的命中率降低20%，持续15秒。该法术只有一个等级。
 - 现在所有达到20级的牧师都将得到防护恐惧结界。持续时间减少到3分钟，冷却时间增加到3分钟。
 - 修正了一个切换出暗影形态会触发公共冷却时间的错误。
 - 神圣之火：修正了等级4的信息框内容。
 - 心灵专注：等级4现在会正常影响光明之泉。
 - 安抚心灵：这个法术将消耗心灵专注。
 - 冥想(戒律天赋)的法力恢复增加到 10/20/30%。
 - 痛苦压制 (戒律天赋)现在可以使用在友好的目标上，立即减少5%目标的仇恨，减少40%受到的伤害，冷却时间缩短为2分钟。
 - 治疗祷言、治疗之环和神圣新星(治疗效果)现在可以得到法伤和治疗效果加成 。
 - 降低了真言术：韧、神圣之灵、坚韧祷言和精神祷言的法力消耗。
 - 真言术：盾现在可以获得法伤和治疗效果加成。等级10、11和12的基础吸收值被降低。
 - 束缚亡灵：如果多个牧师互相覆盖束缚亡灵时，该法术现在始终会被正确去除。
 - 暗言术：灭：韧性不再降低该法术的后续伤害。
 - 星辰碎片(暗夜精灵)不再是持续引导法术，现在它不消耗法力值，是一个持续15秒、每三秒造成一次伤害的魔法效果。冷却时间为30秒。
 - 星辰碎片：在法术生效期间的每跳伤害不再改变。
 - 如果你在暗影形态下试图使用无法在暗影形态下使用的技能，你会自动取消暗影形态，并使用该技能。你可以输入命令/console autoUnshift 0来禁用这一功能。
 - 专注意志(新的戒律系天赋)：在受到爆击之后获得专注意志效果，在6秒内受到的所有伤害降低1/3/5%，受到的治疗效果提高4/7/10%。可叠加3次。
 - 神恩回复：该天赋在牧师坐下时也可触发了。
 - 神佑之韧：该天赋在牧师坐下时也可触发了。
 - 心灵视界：施放时会消耗心灵之火的效果。
 - 愈合祷言：该技能不再被新的低等级愈合祷言效果覆盖。
 - 无声消退：该天赋现在会影响神佑之韧、神恩回复和殉难。
 潜行者
 - 所有药膏在武器上的附魔效果时效从30分钟增加到1小时。
 - 致盲现在被看作是物理攻击(不再是毒性攻击)。材料需求被去除。现在它与飓风术在PVE和PVP中共享递减效果。
 - 健步如飞(刺杀天赋)现在增加8/15%的移动速度。
 - 剑刃乱舞：该技能将正确地对各类带有护罩的目标生效。
 - 精确：该天赋现在同样对远程武器有效。
 - 还击在PVP中现在受到递减效果影响。
 - 冷血：修正了一个当攻击未命中目标时该效果可能被移除的错误。
 - 卑鄙：在目标生命值低于35%时可以造成额外的伤害。
 - 冷酷攻击：该天赋不再会由潜行者目标的假人或者其他宠物的死亡而触发。
 - 潜行者的致命投掷弹道时间将明显增加，缓速时效也将明显增加。
 - 闷棍：多个潜行者的闷棍可以正确地互相取代了。
 - 无情现在只受到近战终结技影响，且不再对致命投掷起作用。
 - 暗影步(敏锐天赋)现在可以在任意状态下使用，而不是只有在潜行状态才可使用。下一次伏击、锁喉或背刺的仇恨降低50%，冷却时间增加到40秒。
 - 装死(敏锐天赋)的触发几率提高到33/66/100%，并且在触发时使潜行者受到的伤害降低90%，持续3秒。该效果每1分钟只能触发一次。
 - 如果你在潜行状态下试图使用无法在潜行状态下使用的技能，你会自动取消潜行状态，并使用该技能。你可以通过输入命令/console autoUnshift 0来禁用这一功能。
 - 卑鄙：该天赋不再因德鲁伊的裂伤和割裂/锁喉技能叠加而受到妨碍。
 - 出血：该技能现在会造成125%的武器伤害。
 萨满祭司
 - 治疗链：调整了等级1-3信息框的内容。
 - 闪电链的施放时间从2.5秒减少到2秒，法力消耗降低，适当地减少了法伤加成。
 - 消毒术和祛病术的范围增加到40码。
 - 大地之盾：如果多个萨满互相覆盖大地之盾，这个法术现在始终会被正确去除。
 - 元素集中(元素战斗天赋)现在降低你下2次伤害法术的法力消耗40%。
 - 冰霜震击不再受到递减效果影响。
 - 根基图腾：该图腾现在会因为吸收法术而被摧毁。
 - 闪电箭的施法时间从3秒减少到2.5秒，法力消耗降低。适当地减少了法伤加成。
 - 闪电掌握(元素战斗天赋)减少你的闪电箭和闪电链.1/.2/.3/.4/.5秒施法时间。
 - 闪电过载(元素战斗天赋)现在有4/8/12/16/20%的几率触发，额外的法术将造成一半的伤害，但是不造成任何威胁值。
 - 增强了法力之泉图腾效果。
 - 精神敏锐(增强天赋)现在同样增加相当于你10/20/30%攻击强度的法术伤害和治疗效果。
 - 所有萨满祭司可以通过相应的武器大师学会使用双手斧和双手锤。增强系天赋被替换成萨满专注。
 - 萨满专注(新的增强天赋)：在一次近战爆击以后，你进入专注状态。专注状态会降低你下一次震击法术60%的法力消耗。
 - 萨满之怒(增强天赋)现在同样会降低30%你在持续时间内所受到的伤害。
 - 灵魂武器(增强天赋) 降低的威胁值从15%增加到30%。
 - 石化武器：从等级4、5、6、7和8这个增强效果得到的伤害将会被略微降低，以符合提示框内所显示的数值。等级1、2、3和9保持不变。
 - 水之护盾：施放这个法术不再消耗法力值，但缩短了它的持续时间。在持续时间结束时，可以从剩余的球体上获得法力值。此外，会显著增加每个球体所提供的法力值。
 - 风怒武器：在被缴械时，不会再触发这个增强效果。
 - 大地之盾：该技能不再被低等级的大地之盾效果覆盖。
 - 元素掌握：该天赋不再必定导致两次连续爆击。
 - 风暴之眼：该天赋在萨满祭司坐下时也可触发了。
 术士
 - 不能再对一个灰名目标(已经被其他玩家或者队伍攻击了)进行放逐。
 - 燃烧：在释放该法术时，将会始终去除献祭的减益性效果。
 - 当术士受到降低治疗效果效果影响时，也会降低吸取生命和生命虹吸的治疗量。(例如致死打击、致伤药膏)
 - 地狱烈焰：该法术不再令敌人的施法时间延长或持续引导的法术时间缩短，同样也不能阻止战场内的夺取军旗。
 - 虚空防护(毁灭天赋)现在有一个新的、更独特的视觉效果。
 - 灵魂仪式的施法和完成时间将明显缩短。
 - 腐蚀之种的爆炸现在将遵循视野限制规则。
 - 暗影之拥：这个天赋效果不再能触发其它效果。
 - 防护暗影结界：这个法术现在得到额外的法术伤害加成。等级4的基础吸收值被降低。
 - 灵魂虹吸(痛苦天赋)：增加伤害2/4%，但不再受到吸取法力影响。
 - 灵魂虹吸(痛苦天赋)：如果术士已经对一个目标施放了特定法术，在其持续时间结束之前重新施放该法术将不会获得额外的效果加成。
 - 痛苦无常：该技能的沉默效果会受到持续时间递减规则的影响。
 战士
 - 冲锋更有可能对在无法行走区域(比如墙和杆子)上的目标起作用。
 - 挑衅(防护天赋)现在同样提供2/4/6的武器精准。
 - 毁灭打击(防护天赋)现在会把破甲的效果计算入它的效果。该技能同样受到所有破甲效果天赋和物品的影响。
 - 缴械在PVP中受到递减效果影响。
 - 强化狂暴姿态(狂暴天赋)现在同时降低2/4/6/8/10%在狂暴姿态下制造的仇恨。
 - 强化拦截和武器掌握在天赋树中交换了位置。
 - 援护不再会让你进入战斗状态。
 - 断筋现在对PVP目标使用有10秒持续时间。
 - 锤类武器专精 (武器天赋)降低了触发几率，但获得的怒气值从6点提升到了7点。
 - 盾牌猛击 (防护天赋)现在总是会尝试驱散目标身上的一个魔法效果。
 - 横扫攻击和死亡之愿在天赋树中交换位置。
 - 横扫攻击(狂暴天赋)现在持续10秒，并影响你的下10次攻击。
 - 横扫攻击：该技能将正确地对各类带有护罩的目标生效。
 - 战术掌握：这个天赋现在同样会显著增加你在防御姿态下致死打击和嗜血的仇恨值。
 - 武器掌握(武器天赋)现在使你受到的缴械效果的持续时间降低25/50%，而不是以前的50%几率避免或全面免疫缴械效果。
 - 旋风斩：当一个战士双持武器时，该技能会使用2把武器来攻击。
 - 血之狂热：该天赋可以在战士坐下时触发了。
 - 挑战怒吼：该技能命中目标的几率会受到命中等级影响了。
 - 激怒：该天赋可以在战士坐下时触发了。
 - 嘲讽：该技能命中目标的几率会受到命中等级影响了。
 专业技能
 - 所有主城以外的主要专业训练师(炼金、锻造、工程学、附魔、制皮、裁缝)都可以提供最高到专家级(300点)的训练。
 - 所有主城的训练师都提供专家级以上的训练，并有一些新的学徒在他们周围。
 - 炼金
 - 加入了一个创造药锅的音效。
 - 新增了一个炼金师传授的配方：疯狂炼金师药水。只有炼金师才能饮用这种新药水。
 - 锻造
 - 增加了一个新的世界掉落的精金武器链配方。该武器链既减少缴械持续时间，又增加招架等级。
 - 磨刀石和平衡石持续1小时。
 - 烹饪
 - 可以从沙塔斯城的巨石洛克那里接到新的日常烹饪任务!完成这些任务，除了金币以外，你还可以得到随机的烹饪材料和新的食谱。
 - 降低了在200点技能到300点技能之间通过食谱获得的技能点数上限。这项改动不包括外域的食谱。
 - 附魔
 - 附魔师制造的巫师之油和法力油的持续时间延长。
 - 稳固附魔现在需要符文魔铁棒(以前是符文恒金棒)。
 - 训练师处新增了一个提高韧性的盾牌附魔配方。
 - 工程学
 - 铁炉堡的考格斯宾和奥格瑞玛的索维克现在会出售限量的小坦克控制器图样。
 - 工程师可以制造新的、不可思议的飞行机器了!去影月谷找到奈奥比·威兹帕克来学习神奇的新图样吧!
 - 制造小坦克控制器所需的材料减少，同时该物品所提供的技能点数也相应降低。
 - 新的修理机器人结构图据说可以从刀锋山的甘尔葛分析师身上找到。
 - 日怒射手会掉落新的制箭设备图样。
 - 精金制弹器可以制作精金弹，每次200发，可使用5次。
 - 工程学药水注射器不再需要工程学技能即可使用。
 - 打架机器人现在是使用后绑定的物品，所以非工程学玩家也可以拥有它们了!
 - 钓鱼
 - 渔夫们现在可以追踪到钓鱼点。这个技能可以从通过钓鱼时偶尔获得的箱子中的书籍上学到。
 - 祖阿曼和逆风小径中可以抓到新种类的鱼，你可以通过日常烹调任务获取这些新类型鱼的食谱。
 - 降低了纳格兰一些区域的钓鱼难度。
 - 你又可以在铁炉堡钓鱼了。
 - 草药学
 - 采药将不会再引起物品效果的触发。
 - 野魔花不再是绑定物品了，但需要275点草药学技能才可使用。
 - 珠宝加工
 - 可以从影月谷的库斯卡海妖那里得到新的天火多彩宝石图样。这个新的多彩宝石提高法术的爆击等级和爆击伤害。
 - 制皮
 - 斯蒂林的帽子系列不再需要斑马皮。
 - 提高了通过制作战鼓获得技能点数的范围。
 - 显著增加了战斗之鼓、恢复之鼓、速度之鼓和战争之鼓的作用范围。
 - 取消了战斗之鼓、恢复之鼓、速度之鼓和战争之鼓的施法时间。
 - 显著增加了苦痛之鼓的作用范围。
 - 降低了学习战争之鼓和苦痛之鼓的声望要求。
 - 战鼓可以在变形状态下使用了。
 - 缩短了苦痛之鼓的施法时间，并增加了一个等同于公共冷却时间的施法时间。
 - 速度之鼓和恢复之鼓不再是世界掉落图样，达到玛格汉/库雷尼崇敬声望后即可在玛格汉/库雷尼商人处购买。
 - 可以从宗师级制皮训练师处学到一个制造增加大量额外护甲值的手套强化物品。该物品是一个永久性的附魔效果，并且不能与其它永久性的附魔叠加。
 - 可以从宗师级制皮训练师处学会制作20格箭袋和子弹包的图样。
 - 可以从荣耀堡、萨尔玛和贫民窟的军需官处买到制造24格箭袋和子弹包的图样。
 - 可以从宗师级制皮训练师处学习新的20格制皮材料袋的图样。
 - 据说壁垒山的食人魔身上有24格制皮材料袋的图样。
 - 采矿
 - 采矿将不会再引起物品效果的触发。
 物品
 - 所有旧世界地下城首领的掉落得到了重新设计，玩家现在可以发现所有副本内掉落的物品质量提高了。
 - 竞技场圣物：给适合使用圣物/圣契/神像的玩家加入了适合所有天赋的圣物/圣契/神像。另外，大部分竞技场系统的圣物/圣契/神像基于一个更合理的命名规范而被改名。
 - 鸦人的羽毛：该物品现在可以在商人处出售。
 - 埃提耶什：该物品不能在战场中打开传送门了。
 - 致命之灰舌符咒：该物品不再在你的终结技对目标无效时触发。
 - 预言之灰舌符咒：增加了通过该饰品获得的法力。
 - 预言之灰舌符咒：该物品不再得到多重机会来触发风暴打击的每次施放。
 - 狂热之灰舌符咒：调整了该饰品从审判上得到的持续时间伤害效果效果，并使之正常工作。缩短了它的持续时间，但它不会被近战攻击重置。
 - 狂热之灰舌符咒：该饰品的持续时间伤害效果不再重置自动攻击和消耗法力。
 - 永恒之戒：修正了放入玩家背包时发出的音效。
 - 永恒治愈者之戒：该物品现在也会提供一些法术伤害加成，因此法术伤害效果可以触发它的效果。另外，持续治疗效果也可以触发它了。
 - 背叛者的黑暗之弓：该物品的触发效果将不再使用误导的次数。另外，触发效果将不会再打破群体控制效果。
 - 血海匪徒外衣：修正了放入玩家背包时的音效。
 - 无尽的痛苦现在可以被分解了。
 - 调整了灾难套装的套装奖励。
 - 消逝贤者圣契：该圣契可以再次正常工作了。
 - 去除了诺格弗格药剂的冷却时间，并不再与其它缩小效果叠加。
 - 仁慈兜帽：修正了放入玩家背包时发出的音效。
 - 调整了晶铸套装的套装奖励。
 - 暗月卡片：复仇：该物品不会再通过周围环境的伤害触发。
 - 死灵精华现在可以从东瘟疫之地的57-60级非精英亡灵身上获得了。
 - 以前是唯一的掉落物品(戒指/饰品/单手武器)现在改为装备唯一。这意味着你可以拥有一个以上的这些物品，但你只能够装备一个。
 - 浓缩泰罗果汁：现在不管你怎么使用该物品，它都能提供法力回复。
 - 破甲虚弱：该天赋的触发效果将不再使用误导的次数。
 - 魔铁宝箱：加强了这些箱子内的物品。
 - 魔誓防毒面具可以在你骑乘状态下戴上时正确生效了。
 - 各个赛季的角斗士系列装备现在将共享套装属性。这意味着你可以使用2件第一赛季装备和2件第2赛季装备来得到4件套装属性。
 - 角斗士的链甲手套：手套上的奖励伤害从4%增加到5%。
 - 大元帅的龙皮头盔：明显增加了这个物品上的智力属性，让它符合部落专有物品的价值。
 - 安图苏尔之手：该物品的触发效果不会再造成战士雷霆一击技能的冷却时间。
 - 浅水权杖：修正了放入玩家背包时的音效。
 - 锁住的箱子现在会显示需要多少开锁技能才能打开。
 - 低等级的布衣物品：所有的低等级布衣物品上的敏捷和力量都被替换成其它属性，通常为法术伤害，但同样也会有一些有益于施法者的其它属性。
 - 决斗者的链甲护手：这些手套的奖励伤害从4%增加到5%。
 - 钳子先生：该物品不会在次数用完以后被摧毁。这解决了当它的最后一次使用次数用完后造成的一些错误。
 - 秘法之天火钻石：给这个物品效果增加了一个冷却时间，不过同时增加了触发几率。
 - 奥格瑞拉声望商人：现在可以用大量埃匹希斯碎片在该商人处购买可以在任何地方使用的药水。
 - 工头伪装服：不再可以在枭兽形态下或者生命之树形态下通过使用这个物品坐上坐骑。
 - 紫罗兰之眼坠饰：这个饰品不会再从一些圣骑士法术的单一施法上触发多次。
 - 影月步兵不再掉落黑暗神殿品质的物品。
 - 希法尔的节点号角：很多法术和技能可以触发这个以前不稳定的物品。
 - 破天套装：四件套装属性现在将被正确应用，两件套装属性会对正确的法术产生作用。
 - 潮汐之怒套装：增加了这套套装通过水之护盾获得的额外法力。
 - 炽热救赎圣典：这个物品不再能通过释放祝福触发。
 - 雷暴图腾：增加了这个物品通过水之护盾获得的额外法力。
 - 不稳定的巫术合剂：这个合剂的信息框将会提供治疗效果加成的详细说明。
 - 终结臂甲：修正了放入玩家背包时的音效。
 - 亡者之眼：该物品现在也提供一些法术伤害加成效果，因此法术伤害效果现在会消耗它的使用次数了。
 - 自由行动药水：该药水提供的增益效果无法被驱散或法术偷取了。
 - 万圣节魔棒：这些物品不再对变形状态下的德鲁伊有效了。
 - 万圣节魔棒：这些物品不能在时光之穴中的旧希尔斯布莱德中使用了。
 - 万圣节南瓜糖：体型增大和橙色效果不再导致作战药剂被移除，并且可以与它们叠加。
 - 电容器：该物品获取电荷时有2.5秒的冷却时间了。
 - 诺格弗格药剂：喝下这种药剂将会引发3秒的非战斗药水冷却时间。
 - 奥格瑞拉声望奖励：所有紫色和蓝色物品都可以分解了。
 - PvP饰品：联盟徽记、部落徽记、联盟勋章和部落勋章都可以解除公正审判的效果了。
 副本与地下城
 - 完成风暴要塞和毒蛇神殿进门钥匙任务的玩家现在将得到一个新的称号，“‘角色名’，纳鲁的勇士”。
 - 英雄副本钥匙的购买需求从崇敬降低到了尊敬。
 - 燃烧的远征前的地下城以外的精英怪被改成了非精英怪。
 - 燃烧的远征前的地下城的等级范围被调整到一个更窄的范围。
 - 集合石等级需求、组队系统和任务被调整为与新的地下城等级范围相符合。
 - 奥金尼地穴
 - 游荡的幽灵现在对玩家是中立。
 - 召唤幽灵现在需要更长的召唤时间。
 - 奥金顿：塞泰克大厅
 - 塞泰克大厅内的怪物现在将一直提供贫民窟声望，直到崇拜。
 - 利爪之王艾吉斯身边的箱子中放有暗影迷宫的钥匙。
 - 奥金顿：暗影迷宫
 - 煽动者布莱卡特：这个怪物的冲锋所导致的昏迷可以正常地受到各类天赋、技能或物品的影响了。
 - 黑暗神殿
 - 高阶灵术师塞勒沃尔的暴风雪和烈焰打击将会在整个效果持续期间生效。
 - 时光之穴：海加尔山
 - 增加了杀死阿克蒙德的奖励声望。
 - 地狱火堡垒：城墙
 - 强化魔铁箱现在只能被参与了某次成功击杀瓦兹德和纳杉的战斗的玩家拾取。另外，在英雄难度下，箱子中会为每个玩家提供一枚公正徽章了。
 - 地狱火堡垒：玛瑟里顿的巢穴
 - 不会在玛瑟里顿被成功放逐前得到心灵疲倦。
 - 卡拉赞
 - 降低了卡拉赞仆役宿舍中的小首领刷新所需杀死的怪物数量。
 - 卡拉赞中的相位猎犬释放相位淡出的几率降低。
 - 冷雾寡妇蛛不会在施放毒箭之雨时清除仇恨。
 - 卡拉赞象棋：尘封的箱子现在只能被事件完成时在场的玩家拾取。另外，箱子中会为每个玩家提供2枚公正徽章。
 - 泰坦之怒不再打破束缚亡灵。
 - 风暴要塞
 - 在风暴要塞架桥上巡逻的血精灵不会再巡逻到凯尔萨斯逐日者的房间。
 - 使用寒冰屏障、神圣之盾、暗影斗篷和其它类似法术将不再引爆大星术师索兰莉安的星术师之怒。
 - 凯尔萨斯
 - - 四个顾问的血量都减少10%。
 - - 所有被召唤武器的血量减少10%。
 - 奥
 - - 技能“流星”被改名为 “俯冲轰炸”。
 - 能源舰
 - 灵术师塞比瑟蕾的冰霜攻击现在降低25%移动速度，而不是之前的50%。
 - 军团储藏箱中的物品现在只能被参与了某次成功击杀看守者盖罗基尔和看守者埃隆汉的玩家拾取。另外，在英雄模式下这两个怪物不再掉落公正徽章，但是军团储藏箱中会有一枚。
 任务
 - 增加了新的针对外域五人副本和时光之穴的英雄模式和普通模式的随机日常副本任务，每天会随机有针对普通模式副本和英雄模式副本的各一个。星界财团任务发布者就在沙塔斯城的贫民窟。
 - 现在可以更早地开始捕捉以太鳐。
 - 任务”电池充电“中使用的电池充能器的使用范围增加。
 - 给予日常任务的NPC头上将显示蓝色感叹号。
 - 尘泥沼泽
 - 在尘泥沼泽加入了很多新的任务。
 - 热砂港的地精们刚在尘泥沼泽的西南完成了新城镇泥链镇的建造。
 - 增加了1级至60级地下城任务的经验和声望奖励。
 - 很多在1级至60级过程中的精英怪物和任务被修改，以更适合于单人游戏。
 - 沙塔斯城的飞行管理员现在会给70级但没有225骑术的玩家一个任务，任务会指引角色去影月谷拜访相应的骑术教师，因此玩家可以知道在哪里学习使用飞行坐骑所需的骑术。
 - 改变了充能的聚焦水晶的图标。
 - 极化电磁球现在只在刀锋山有效。
 - 在完成幽魂之地的任务“香料”后，如果角色仍然拥有药剂师的毒药，那么将会从他们的物品栏里面移除这个物品。
 用户界面
 - 改进了拍卖行UI。去除了没用的分类(比如说板甲-披风)并加入了许多额外的辅助分类。另外，改变了很多物品的分类，让它们更有意义。
 - 改变了拍卖行的拍卖周期。现在物品可以进行12、24或48小时的拍卖，取消了8小时的拍卖选项。
 - 作战地图被改名成区域地图，并可以在任何地区打开。 在世界地图界面上会有一个下拉菜单，供你选择在什么时候显示区域地图。
 - 拥有冷却时间的物品现在会在提示栏里面显示基础的冷却时间。
 - 可以互动的物体现在会有光芒包围，并会在顶部显示它们的名字，从而使这些物体变得更明显。
 - 给予任务的物体顶部会有一个感叹号标志，而完成任务的物体顶部会有一个问号标志。
 - 插有多彩宝石的物品现在会显示需要激活多彩宝石所需的宝石，并在提示栏里面显示是否达到宝石要求。
 - 在角色面板上的格挡几率条目也会显示格挡值。
 - 观察其他玩家装备的距离被增加到30码。
 - 在你观察另一个玩家时，除了可以看到他们的装备外，还能看到他们的天赋选择。
 - 重新修改了精神控制和占据的动作条。
 - 所有玩家的微缩地图旁都有一个新的追踪按钮。你可以通过此选单选择寻找某个特定类型的商人或者NPC。如果你的角色拥有追踪技能，你也可以在那里选择某个特定的追踪类型。这就意味着猎人可以从他们的动作条上完全去掉各类追踪按钮。
 - 现在可以选择是否在你的生命/法力条上向你/你的队友显示你的生命/法力值，并可以选择以百分比或是数值形式显示。
 - 当你控制一个目标时(牧师的精神控制、野兽之眼、使用蒸汽车控制器等等)，被控制目标的动作条将取代你的主动作条，而不是在你的普通动作条上显示一条微型动作条。
 - 当一个物品掉落而由于物品唯一性或者其他因素你不能拾取时，你将得到一个聊天信息让你得知你已经放弃了对该物品的掷骰。
 - 如果一个玩家正在拾取尸体，而在该尸体上有着一件或者多件该玩家不能拾取的物品，玩家将仍然可以看到这些物品(尽管并不能拾取)。这个改动只针对绿色或者更高质量的物品。该改动可以让其他玩家察看到只有该商业技能的玩家才能拾取的商业技能配方，或者查看在队长分配情况下尸体上的物品。
 - 拥有与等级相符的任务的NPC将以黄色感叹号的形式显示在你的微缩地图上。已经完成的任务的相应NPC将以一个黄色问号形式显示在你的微缩地图上，而不是以前的黄点。另外，你尚未获得飞行点的飞行管理员现在将以一个绿色感叹号的形式显示在你的微缩地图上。
 - 现在将鼠标移到NPC上，将会显示更直观的指示来表明他们相应的功能以取代以前的对话提示。将鼠标移到给予任务的NPC身上将会显示一个问号，将鼠标移到旅店老板身上将会显示一个炉石符号，诸如此类。
 - 简化了拍卖行的分类搜索，这样你可以只排序一条内容，而且为这些条目创建的自动附属排列将变得更加有用。另外，排序现在将在服务器端完成，因此当你翻页查看结果时，物品会以全部结果的顺序排列而不是以前只排列你正在查看的一页结果。
 - 法术细节设置被重置为默认设置。2.3以前针对所有系统的默认设置都是并不是适用于所有硬件的最大效果。
 - 如果你得到一条“插件导致界面功能失效”的信息，说明某个插件在某些方面与Blizzard UI有所冲突。可以通过在聊天窗口输入“/console taintLog 1"重新启动游戏激活该命令来诊断问题。如果你激活了这个命令，你可以在退出游戏后，在Logs\Taint.log文件内搜索关键词"Blocked"，然后将记录邮寄给插件的作者。在完成这个步骤以后，你可以通过在聊天窗口输入/console taintLog 0来关闭日志记录。
 - 你现在可以在一封邮件中寄送最多12个物品。
 - 在你打开一份未发送邮件时，你可以右键点击物品将他们拖入那封未发送邮件。
 - 在你有交易窗口打开时，你可以右键点击一个物品将它移入交易窗口。
 - 当你拥有未开启的邮件时，你可以将鼠标移到邮件图标，以了解最新的邮件是谁寄来的。
 - 当你和你一个只有单一功能的NPC对话(银行管理员，飞行管理员，等等)和其他没有闲谈选项或任务的NPC时，你将可以直接选择他们的功能。这意味着点击飞机管理员会直接显示飞行路线地图，而不是一个带有进入飞行路线地图选项的闲聊界面。
 - 远低于你等级的任务现在会在获取任务闲聊界面出现一个(低等级)的标识。这样会更简单的让你在一个同样拥有低等级和高等级任务的NPC出知道你该接哪个任务
 - 你现在可以用一个词或一个数字来设定拾取下限(/threshold)，举例来说，/threshold Epic可以置队伍的拾取下限为史诗级物品，或者质量更好的物品。/threshold Rare可以设置队伍的拾取下限为稀有治疗的物品或者质量更好的物品。
 - 将鼠标指在尸体上会显示队伍中的谁可以拾取。这个改变可以帮助剥皮师告诉谁去拾取，然后他就可以去剥皮了。同样在队长分配情况下，当掉落了一件特别的物品时，每个人都会看到怪物尸体上显示物品分配者拥有拾取的权力。
 - 加入了一个“使用硬件”的选项。这个选项是针对使用特定硬件配置的高级用户的，它可能会明显改变你的游戏运行时的声音效果。
 - 从团队界面拉出到游戏界面的内容现在可以在进程中保存设置。
 - 当团队首领发送了一条重要的信息，在你的屏幕中间会出现一个更大、更容易注意的效果。
 - 团队警告命令现在会以在屏幕中间显示发光的巨大的文字并会略微收缩让你更容易注意。
 - 现在将鼠标移在只有单一功能的NPC身上时，会显示一个与他们功能相关的图标。
 - 当插件尝试执行一个不能在战斗状态下执行的动作时，你会得到一个“插件导致界面功能失败”的信息，每次登录只会显示一次。
 - 很多界面可以用职业颜色来显示玩家，这样你可以简单的分辨该玩家是什么职业。这个功能将应用于组队、聊天频道UI、公会和查询。
 - TargetNearestDistance设置的数值在读取时间内被验证。
 - 视频选项将可以保存非标准的刷新率。
 - 很多可以使用的物品现在在物品提示框内显示冷却时间。
 - 在角色面板上现在可以显示一次成功的格挡可以降低多少伤害。
 - 新的宏命令：/targetlastenemy, /targetlastfriend
 - 新的通过目标全名的宏命令：/targetexact
 - 想要了解更多UI宏和脚本的变化，请查看暴雪的UI和宏论坛。
 世界环境
 - 怪物AI得到了改变，不再无视被恐惧的目标去攻击未被恐惧的目标。
 - 纳格兰的空气云雾现在会产生空气微粒。
 - 赞加沼泽的空气云雾现在产生水之微粒，而不是生命微粒。
 - 虚空风暴的空气云雾变为总是可见的了。护目镜上的探测效果改为在微缩地图上显示附近的空气云雾的位置，可以与其它追踪类型叠加。
 - 荆棘谷
 - - 盐鳞首领不再施放冰霜新星。
 - - 减少了莫什奥格领主的恐惧时间。
 - 降低了迪菲亚抢劫者的火球伤害。
 - 增加了库尔森药草师治疗的冷却时间。
 - 很多怪物使用的缴械技能的持续时间从6秒改为5秒。
 - 音素爆破的沉默时间从10秒改为6秒。
 错误修正
 - 修正了一个铸魔营地：恐怖和铸魔营地：天罚刷新速度的问题。
 - 修正了任务“偷回蘑菇”的某个“一箱蘑菇”物品在安葛洛什要塞的位置。
 - 修正了刀锋山一个漂浮在空中的矿点位置。
 - 修正了任务“终极挑战”中的方向问题。
 - 修正了在宠物界面中，把鼠标移上耐力提示框显示不正确血量增加值的错误。
 - 修正了被控制以后的光环叠加和减益效果冷却时间的问题。
 - 当另一个玩家施放了新的增益法术之后，反转冷却时间将正确刷新。

 * DECLARE_KFIFO - macro to declare a fifo object
 * @fifo: name of the declared fifo
 * @type: type of the fifo elements
 * @size: the number of elements in the fifo, this must be a power of 2
 */
#define DECLARE_KFIFO(fifo, type, size)	STRUCT_KFIFO(type, size) fifo

/**
 * INIT_KFIFO - Initialize a fifo declared by DECLARE_KFIFO
 * @fifo: name of the declared fifo datatype
 */
#define INIT_KFIFO(fifo) \
(void)({ \
	typeof(&(fifo)) __tmp = &(fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	__kfifo->in = 0; \
	__kfifo->out = 0; \
	__kfifo->mask = __is_kfifo_ptr(__tmp) ? 0 : ARRAY_SIZE(__tmp->buf) - 1;\
	__kfifo->esize = sizeof(*__tmp->buf); \
	__kfifo->data = __is_kfifo_ptr(__tmp) ?  NULL : __tmp->buf; \
})

/**
 * DEFINE_KFIFO - macro to define and initialize a fifo
 * @fifo: name of the declared fifo datatype
 * @type: type of the fifo elements
 * @size: the number of elements in the fifo, this must be a power of 2
 *
 * Note: the macro can be used for global and local fifo data type variables.
 */
#define DEFINE_KFIFO(fifo, type, size) \
	DECLARE_KFIFO(fifo, type, size) = \
	(typeof(fifo)) { \
		{ \
			{ \
			.in	= 0, \
			.out	= 0, \
			.mask	= __is_kfifo_ptr(&(fifo)) ? \
				  0 : \
				  ARRAY_SIZE((fifo).buf) - 1, \
			.esize	= sizeof(*(fifo).buf), \
			.data	= __is_kfifo_ptr(&(fifo)) ? \
				NULL : \
				(fifo).buf, \
			} \
		} \
	}


static inline unsigned int __must_check
__kfifo_uint_must_check_helper(unsigned int val)
{
	return val;
}

static inline int __must_check
__kfifo_int_must_check_helper(int val)
{
	return val;
}

/**
 * kfifo_initialized - Check if the fifo is initialized
 * @fifo: address of the fifo to check
 *
 * Return %true if fifo is initialized, otherwise %false.
 * Assumes the fifo was 0 before.
 */
#define kfifo_initialized(fifo) ((fifo)->kfifo.mask)

/**
 * kfifo_esize - returns the size of the element managed by the fifo
 * @fifo: address of the fifo to be used
 */
#define kfifo_esize(fifo)	((fifo)->kfifo.esize)

/**
 * kfifo_recsize - returns the size of the record length field
 * @fifo: address of the fifo to be used
 */
#define kfifo_recsize(fifo)	(sizeof(*(fifo)->rectype))

/**
 * kfifo_size - returns the size of the fifo in elements
 * @fifo: address of the fifo to be used
 */
#define kfifo_size(fifo)	((fifo)->kfifo.mask + 1)

/**
 * kfifo_reset - removes the entire fifo content
 * @fifo: address of the fifo to be used
 *
 * Note: usage of kfifo_reset() is dangerous. It should be only called when the
 * fifo is exclusived locked or when it is secured that no other thread is
 * accessing the fifo.
 */
#define kfifo_reset(fifo) \
(void)({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	__tmp->kfifo.in = __tmp->kfifo.out = 0; \
})

/**
 * kfifo_reset_out - skip fifo content
 * @fifo: address of the fifo to be used
 *
 * Note: The usage of kfifo_reset_out() is safe until it will be only called
 * from the reader thread and there is only one concurrent reader. Otherwise
 * it is dangerous and must be handled in the same way as kfifo_reset().
 */
#define kfifo_reset_out(fifo)	\
(void)({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	__tmp->kfifo.out = __tmp->kfifo.in; \
})

/**
 * kfifo_len - returns the number of used elements in the fifo
 * @fifo: address of the fifo to be used
 */
#define kfifo_len(fifo) \
({ \
	typeof((fifo) + 1) __tmpl = (fifo); \
	__tmpl->kfifo.in - __tmpl->kfifo.out; \
})

/**
 * kfifo_is_empty - returns true if the fifo is empty
 * @fifo: address of the fifo to be used
 */
#define	kfifo_is_empty(fifo) \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	__tmpq->kfifo.in == __tmpq->kfifo.out; \
})

/**
 * kfifo_is_full - returns true if the fifo is full
 * @fifo: address of the fifo to be used
 */
#define	kfifo_is_full(fifo) \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	kfifo_len(__tmpq) > __tmpq->kfifo.mask; \
})

/**
 * kfifo_avail - returns the number of unused elements in the fifo
 * @fifo: address of the fifo to be used
 */
#define	kfifo_avail(fifo) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmpq = (fifo); \
	const size_t __recsize = sizeof(*__tmpq->rectype); \
	unsigned int __avail = kfifo_size(__tmpq) - kfifo_len(__tmpq); \
	(__recsize) ? ((__avail <= __recsize) ? 0 : \
	__kfifo_max_r(__avail - __recsize, __recsize)) : \
	__avail; \
}) \
)

/**
 * kfifo_skip - skip output data
 * @fifo: address of the fifo to be used
 */
#define	kfifo_skip(fifo) \
(void)({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__kfifo_skip_r(__kfifo, __recsize); \
	else \
		__kfifo->out++; \
})

/**
 * kfifo_peek_len - gets the size of the next fifo record
 * @fifo: address of the fifo to be used
 *
 * This function returns the size of the next fifo record in number of bytes.
 */
#define kfifo_peek_len(fifo) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(!__recsize) ? kfifo_len(__tmp) * sizeof(*__tmp->type) : \
	__kfifo_len_r(__kfifo, __recsize); \
}) \
)

/**
 * kfifo_alloc - dynamically allocates a new fifo buffer
 * @fifo: pointer to the fifo
 * @size: the number of elements in the fifo, this must be a power of 2
 * @gfp_mask: get_free_pages mask, passed to kmalloc()
 *
 * This macro dynamically allocates a new fifo buffer.
 *
 * The number of elements will be rounded-up to a power of 2.
 * The fifo will be release with kfifo_free().
 * Return 0 if no error, otherwise an error code.
 */
#define kfifo_alloc(fifo, size, gfp_mask) \
__kfifo_int_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	__is_kfifo_ptr(__tmp) ? \
	__kfifo_alloc(__kfifo, size, sizeof(*__tmp->type), gfp_mask) : \
	-EINVAL; \
}) \
)

/**
 * kfifo_free - frees the fifo
 * @fifo: the fifo to be freed
 */
#define kfifo_free(fifo) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__is_kfifo_ptr(__tmp)) \
		__kfifo_free(__kfifo); \
})

/**
 * kfifo_init - initialize a fifo using a preallocated buffer
 * @fifo: the fifo to assign the buffer
 * @buffer: the preallocated buffer to be used
 * @size: the size of the internal buffer, this have to be a power of 2
 *
 * This macro initializes a fifo using a preallocated buffer.
 *
 * The number of elements will be rounded-up to a power of 2.
 * Return 0 if no error, otherwise an error code.
 */
#define kfifo_init(fifo, buffer, size) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	__is_kfifo_ptr(__tmp) ? \
	__kfifo_init(__kfifo, buffer, size, sizeof(*__tmp->type)) : \
	-EINVAL; \
})

/**
 * kfifo_put - put data into the fifo
 * @fifo: address of the fifo to be used
 * @val: the data to be added
 *
 * This macro copies the given value into the fifo.
 * It returns 0 if the fifo was full. Otherwise it returns the number
 * processed elements.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_put(fifo, val) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(*__tmp->const_type) __val = (val); \
	unsigned int __ret; \
	size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__ret = __kfifo_in_r(__kfifo, &__val, sizeof(__val), \
			__recsize); \
	else { \
		__ret = !kfifo_is_full(__tmp); \
		if (__ret) { \
			(__is_kfifo_ptr(__tmp) ? \
			((typeof(__tmp->type))__kfifo->data) : \
			(__tmp->buf) \
			)[__kfifo->in & __tmp->kfifo.mask] = \
				*(typeof(__tmp->type))&__val; \
			smp_wmb(); \
			__kfifo->in++; \
		} \
	} \
	__ret; \
})

/**
 * kfifo_get - get data from the fifo
 * @fifo: address of the fifo to be used
 * @val: address where to store the data
 *
 * This macro reads the data from the fifo.
 * It returns 0 if the fifo was empty. Otherwise it returns the number
 * processed elements.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_get(fifo, val) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __val = (val); \
	unsigned int __ret; \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__ret = __kfifo_out_r(__kfifo, __val, sizeof(*__val), \
			__recsize); \
	else { \
		__ret = !kfifo_is_empty(__tmp); \
		if (__ret) { \
			*(typeof(__tmp->type))__val = \
				(__is_kfifo_ptr(__tmp) ? \
				((typeof(__tmp->type))__kfifo->data) : \
				(__tmp->buf) \
				)[__kfifo->out & __tmp->kfifo.mask]; \
			smp_wmb(); \
			__kfifo->out++; \
		} \
	} \
	__ret; \
}) \
)

/**
 * kfifo_peek - get data from the fifo without removing
 * @fifo: address of the fifo to be used
 * @val: address where to store the data
 *
 * This reads the data from the fifo without removing it from the fifo.
 * It returns 0 if the fifo was empty. Otherwise it returns the number
 * processed elements.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_peek(fifo, val) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __val = (val); \
	unsigned int __ret; \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__ret = __kfifo_out_peek_r(__kfifo, __val, sizeof(*__val), \
			__recsize); \
	else { \
		__ret = !kfifo_is_empty(__tmp); \
		if (__ret) { \
			*(typeof(__tmp->type))__val = \
				(__is_kfifo_ptr(__tmp) ? \
				((typeof(__tmp->type))__kfifo->data) : \
				(__tmp->buf) \
				)[__kfifo->out & __tmp->kfifo.mask]; \
			smp_wmb(); \
		} \
	} \
	__ret; \
}) \
)

/**
 * kfifo_in - put data into the fifo
 * @fifo: address of the fifo to be used
 * @buf: the data to be added
 * @n: number of elements to be added
 *
 * This macro copies the given buffer into the fifo and returns the
 * number of copied elements.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_in(fifo, buf, n) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr_const) __buf = (buf); \
	unsigned long __n = (n); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ?\
	__kfifo_in_r(__kfifo, __buf, __n, __recsize) : \
	__kfifo_in(__kfifo, __buf, __n); \
})

/**
 * kfifo_in_spinlocked - put data into the fifo using a spinlock for locking
 * @fifo: address of the fifo to be used
 * @buf: the data to be added
 * @n: number of elements to be added
 * @lock: pointer to the spinlock to use for locking
 *
 * This macro copies the given values buffer into the fifo and returns the
 * number of copied elements.
 */
 #if 0
#define	kfifo_in_spinlocked(fifo, buf, n, lock) \
({ \
	unsigned long __flags; \
	unsigned int __ret; \
	spin_lock_irqsave(lock, __flags); \
	__ret = kfifo_in(fifo, buf, n); \
	spin_unlock_irqrestore(lock, __flags); \
	__ret; \
})

/* alias for kfifo_in_spinlocked, will be removed in a future release */
#define kfifo_in_locked(fifo, buf, n, lock) \
		kfifo_in_spinlocked(fifo, buf, n, lock)
#endif
/**
 * kfifo_out - get data from the fifo
 * @fifo: address of the fifo to be used
 * @buf: pointer to the storage buffer
 * @n: max. number of elements to get
 *
 * This macro get some data from the fifo and return the numbers of elements
 * copied.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_out(fifo, buf, n) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __buf = (buf); \
	unsigned long __n = (n); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ?\
	__kfifo_out_r(__kfifo, __buf, __n, __recsize) : \
	__kfifo_out(__kfifo, __buf, __n); \
}) \
)

/**
 * kfifo_out_spinlocked - get data from the fifo using a spinlock for locking
 * @fifo: address of the fifo to be used
 * @buf: pointer to the storage buffer
 * @n: max. number of elements to get
 * @lock: pointer to the spinlock to use for locking
 *
 * This macro get the data from the fifo and return the numbers of elements
 * copied.
 */
#define	kfifo_out_spinlocked(fifo, buf, n, lock) \
__kfifo_uint_must_check_helper( \
({ \
	unsigned long __flags; \
	unsigned int __ret; \
	spin_lock_irqsave(lock, __flags); \
	__ret = kfifo_out(fifo, buf, n); \
	spin_unlock_irqrestore(lock, __flags); \
	__ret; \
}) \
)

/* alias for kfifo_out_spinlocked, will be removed in a future release */
#define kfifo_out_locked(fifo, buf, n, lock) \
		kfifo_out_spinlocked(fifo, buf, n, lock)

/**
 * kfifo_from_user - puts some data from user space into the fifo
 * @fifo: address of the fifo to be used
 * @from: pointer to the data to be added
 * @len: the length of the data to be added
 * @copied: pointer to output variable to store the number of copied bytes
 *
 * This macro copies at most @len bytes from the @from into the
 * fifo, depending of the available space and returns -EFAULT/0.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_from_user(fifo, from, len, copied) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	const void __user *__from = (from); \
	unsigned int __len = (len); \
	unsigned int *__copied = (copied); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ? \
	__kfifo_from_user_r(__kfifo, __from, __len,  __copied, __recsize) : \
	__kfifo_from_user(__kfifo, __from, __len, __copied); \
}) \
)

/**
 * kfifo_to_user - copies data from the fifo into user space
 * @fifo: address of the fifo to be used
 * @to: where the data must be copied
 * @len: the size of the destination buffer
 * @copied: pointer to output variable to store the number of copied bytes
 *
 * This macro copies at most @len bytes from the fifo into the
 * @to buffer and returns -EFAULT/0.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_to_user(fifo, to, len, copied) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	void __user *__to = (to); \
	unsigned int __len = (len); \
	unsigned int *__copied = (copied); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ? \
	__kfifo_to_user_r(__kfifo, __to, __len, __copied, __recsize) : \
	__kfifo_to_user(__kfifo, __to, __len, __copied); \
}) \
)

/**
 * kfifo_dma_in_prepare - setup a scatterlist for DMA input
 * @fifo: address of the fifo to be used
 * @sgl: pointer to the scatterlist array
 * @nents: number of entries in the scatterlist array
 * @len: number of elements to transfer
 *
 * This macro fills a scatterlist for DMA input.
 * It returns the number entries in the scatterlist array.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macros.
 */
#define	kfifo_dma_in_prepare(fifo, sgl, nents, len) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct scatterlist *__sgl = (sgl); \
	int __nents = (nents); \
	unsigned int __len = (len); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ? \
	__kfifo_dma_in_prepare_r(__kfifo, __sgl, __nents, __len, __recsize) : \
	__kfifo_dma_in_prepare(__kfifo, __sgl, __nents, __len); \
})

/**
 * kfifo_dma_in_finish - finish a DMA IN operation
 * @fifo: address of the fifo to be used
 * @len: number of bytes to received
 *
 * This macro finish a DMA IN operation. The in counter will be updated by
 * the len parameter. No error checking will be done.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macros.
 */
#define kfifo_dma_in_finish(fifo, len) \
(void)({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	unsigned int __len = (len); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__kfifo_dma_in_finish_r(__kfifo, __len, __recsize); \
	else \
		__kfifo->in += __len / sizeof(*__tmp->type); \
})

/**
 * kfifo_dma_out_prepare - setup a scatterlist for DMA output
 * @fifo: address of the fifo to be used
 * @sgl: pointer to the scatterlist array
 * @nents: number of entries in the scatterlist array
 * @len: number of elements to transfer
 *
 * This macro fills a scatterlist for DMA output which at most @len bytes
 * to transfer.
 * It returns the number entries in the scatterlist array.
 * A zero means there is no space available and the scatterlist is not filled.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macros.
 */
#define	kfifo_dma_out_prepare(fifo, sgl, nents, len) \
({ \
	typeof((fifo) + 1) __tmp = (fifo);  \
	struct scatterlist *__sgl = (sgl); \
	int __nents = (nents); \
	unsigned int __len = (len); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ? \
	__kfifo_dma_out_prepare_r(__kfifo, __sgl, __nents, __len, __recsize) : \
	__kfifo_dma_out_prepare(__kfifo, __sgl, __nents, __len); \
})

/**
 * kfifo_dma_out_finish - finish a DMA OUT operation
 * @fifo: address of the fifo to be used
 * @len: number of bytes transferred
 *
 * This macro finish a DMA OUT operation. The out counter will be updated by
 * the len parameter. No error checking will be done.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macros.
 */
#define kfifo_dma_out_finish(fifo, len) \
(void)({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	unsigned int __len = (len); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (__recsize) \
		__kfifo_dma_out_finish_r(__kfifo, __recsize); \
	else \
		__kfifo->out += __len / sizeof(*__tmp->type); \
})

/**
 * kfifo_out_peek - gets some data from the fifo
 * @fifo: address of the fifo to be used
 * @buf: pointer to the storage buffer
 * @n: max. number of elements to get
 *
 * This macro get the data from the fifo and return the numbers of elements
 * copied. The data is not removed from the fifo.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these macro.
 */
#define	kfifo_out_peek(fifo, buf, n) \
__kfifo_uint_must_check_helper( \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof(__tmp->ptr) __buf = (buf); \
	unsigned long __n = (n); \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	(__recsize) ? \
	__kfifo_out_peek_r(__kfifo, __buf, __n, __recsize) : \
	__kfifo_out_peek(__kfifo, __buf, __n); \
}) \
)
// liigo: we dont have gfp_t
// extern int __kfifo_alloc(struct __kfifo *fifo, unsigned int size,
// 	size_t esize, gfp_t gfp_mask);

// extern void __kfifo_free(struct __kfifo *fifo);

extern int __kfifo_init(struct __kfifo *fifo, void *buffer,
	unsigned int size, size_t esize);

extern unsigned int __kfifo_in(struct __kfifo *fifo,
	const void *buf, unsigned int len);

extern unsigned int __kfifo_out(struct __kfifo *fifo,
	void *buf, unsigned int len);

/// comment following by liigo

// extern int __kfifo_from_user(struct __kfifo *fifo,
// 	const void __user *from, unsigned long len, unsigned int *copied);

// extern int __kfifo_to_user(struct __kfifo *fifo,
// 	void __user *to, unsigned long len, unsigned int *copied);

// extern unsigned int __kfifo_dma_in_prepare(struct __kfifo *fifo,
// 	struct scatterlist *sgl, int nents, unsigned int len);

// extern unsigned int __kfifo_dma_out_prepare(struct __kfifo *fifo,
// 	struct scatterlist *sgl, int nents, unsigned int len);

extern unsigned int __kfifo_out_peek(struct __kfifo *fifo,
	void *buf, unsigned int len);

extern unsigned int __kfifo_in_r(struct __kfifo *fifo,
	const void *buf, unsigned int len, size_t recsize);

extern unsigned int __kfifo_out_r(struct __kfifo *fifo,
	void *buf, unsigned int len, size_t recsize);

// extern int __kfifo_from_user_r(struct __kfifo *fifo,
// 	const void __user *from, unsigned long len, unsigned int *copied,
// 	size_t recsize);

// extern int __kfifo_to_user_r(struct __kfifo *fifo, void __user *to,
// 	unsigned long len, unsigned int *copied, size_t recsize);

// extern unsigned int __kfifo_dma_in_prepare_r(struct __kfifo *fifo,
// 	struct scatterlist *sgl, int nents, unsigned int len, size_t recsize);

// extern void __kfifo_dma_in_finish_r(struct __kfifo *fifo,
// 	unsigned int len, size_t recsize);

// extern unsigned int __kfifo_dma_out_prepare_r(struct __kfifo *fifo,
// 	struct scatterlist *sgl, int nents, unsigned int len, size_t recsize);

// extern void __kfifo_dma_out_finish_r(struct __kfifo *fifo, size_t recsize);

extern unsigned int __kfifo_len_r(struct __kfifo *fifo, size_t recsize);

extern void __kfifo_skip_r(struct __kfifo *fifo, size_t recsize);

extern unsigned int __kfifo_out_peek_r(struct __kfifo *fifo,
	void *buf, unsigned int len, size_t recsize);

extern unsigned int __kfifo_max_r(unsigned int len, size_t recsize);

#endif

