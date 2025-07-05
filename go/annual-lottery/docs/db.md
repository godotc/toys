## 数据库设计

- 奖品
- 优惠券
- 中奖记录
- 用户黑名单
- IP 黑名单
- 用户参与次数

### 用户表

```xml
<table name="user">
    <field name="id" type="int" description="主键，自增"/>
    <field name="username" type="varchar(50)" description="用户名"/>
    <field name="real_name" type="varchar(50)" description="真实姓名"/>
    <field name="phone" type="varchar(20)" description="手机号"/>
    <field name="address" type="varchar(255)" description="地址"/>

    <field name="sys_status" type="small_int" description="系统状态，0-正常，1-已删除"/>
    <field name="sys_created" type="int" description="创建时间，Unix 时间戳"/>
    <field name="sys_updated" type="int" description="更新时间，Unix 时间戳"/>
</table>

```

<details>
  <summary>SQL for postgres - 用户表</summary>

```sql
CREATE TABLE IF NOT EXISTS "user" (
  "id" SERIAL PRIMARY KEY,
  "username" VARCHAR(50) NOT NULL UNIQUE,
  "realname" VARCHAR(50),
  "phone" VARCHAR(20) UNIQUE,
  "address" VARCHAR(255),
  "sys_status" SMALLINT NOT NULL DEFAULT 0,
  "sys_created" INT NOT NULL,
  "sys_updated" INT NOT NULL
);

-- 创建索引
CREATE INDEX IF NOT EXISTS "idx_user_sys_status" ON "user" ("sys_status");
```

</details>

### 奖品表

```xml
<table name="gift">
    <field name="id" type="int" description="主键，自增"/>
    <field name="title" type="varchar(255)" description="奖品名称"/>
    <field name="prize_num" type="int" description="奖品数量"/>
    <field name="left_num" type="int" description="剩余奖品数量"/>
    <field name="prize_code" type="int" description="概率为倒数: 1/1000, 1/500, 1/100"/>
    <field name="prize_time" type="int" description="发奖周期，单位 Day"/>
    <field name="img" type="varchar(255)" description="奖品图片 URL"/>
    <field name="display_order" type="int" description="显示顺序，越小越靠前"/>
    <field name="gtype" type="int" description="奖品类型，0-虚拟币，1-虚拟券，2-实物小奖，3-实物大奖"/>
    <field name="gdata" type="varchar(255)" description="奖品数据，JSON 格式，包含券码、实物地址等"/>
    <field name="time_begin" type="int" description="开始时间，Unix 时间戳"/>
    <field name="time_end" type="int" description="结束时间，Unix 时间戳"/>
    <field name="prize_data" type="mediumtext" description="发奖计划，JSON 格式，包含每个奖品的发放时间和数量"/>
    <field name="prize_begin" type="int" description="发奖开始时间，Unix 时间戳"/>
    <field name="prize_end" type="int" description="发奖结束时间，Unix 时间戳"/>

    <field name="sys_status" type="small_int" description="系统状态，0-正常，1-已删除，2-已下架"/>
    <field name="sys_created" type="int" description="创建时间，Unix 时间戳"/>
    <field name="sys_updated" type="int" description="更新时间，Unix 时间戳"/>
    <field name="sys_ip" type="varchar(50)" description="创建人"/>
</table>
```

<details>
  <summary>SQL for postgres - 奖品表</summary>

```sql
CREATE TABLE IF NOT EXISTS "gift" (
  "id" SERIAL PRIMARY KEY,
  "title" VARCHAR(255) NOT NULL,
  "prize_num" INT NOT NULL,
  "left_num" INT NOT NULL,
  "prize_code" INT NOT NULL,
  "prize_time" INT NOT NULL,
  "img" VARCHAR(255) NOT NULL,
  "display_order" INT NOT NULL,
  "gtype" INT NOT NULL,
  "gdata" VARCHAR(255) NOT NULL DEFAULT '{}',
  "time_begin" INT NOT NULL,
  "time_end" INT NOT NULL,
  "prize_data" TEXT NOT NULL,
  "prize_begin" INT NOT NULL,
  "prize_end" INT NOT NULL,
  "sys_status" SMALLINT NOT NULL DEFAULT 0,
  "sys_created" INT NOT NULL,
  "sys_updated" INT NOT NULL,
  "sys_ip" VARCHAR(50) NOT NULL
);

-- 创建索引
CREATE INDEX IF NOT EXISTS "idx_gift_gtype" ON "gift" ("gtype");
CREATE INDEX IF NOT EXISTS "idx_gift_display_order" ON "gift" ("display_order");
CREATE INDEX IF NOT EXISTS "idx_gift_time_range" ON "gift" ("time_begin", "time_end");
CREATE INDEX IF NOT EXISTS "idx_gift_prize_range" ON "gift" ("prize_begin", "prize_end");
CREATE INDEX IF NOT EXISTS "idx_gift_sys_status" ON "gift" ("sys_status");
```

</details>

### 优惠券表

```xml
<table name="coupon">
    <field name="id" type="int" description="主键，自增"/>
    <field name="gift_id" type="int" description="奖品 ID，关联奖品表"/>
    <field name="code" type="varchar(50)" description="券码，唯一标识优惠券"/>

    <field name="sys_status" type="small_int" description="系统状态，0-正常，1-已删除，2-已下架"/>
    <field name="sys_created" type="int" description="创建时间，Unix 时间戳"/>
    <field name="sys_updated" type="int" description="更新时间，Unix 时间戳"/>
</table>
```

<details>
  <summary>SQL for postgres - 优惠券表</summary>

```sql
CREATE TABLE IF NOT EXISTS "coupon" (
  "id" SERIAL PRIMARY KEY,
  "gift_id" INT NOT NULL REFERENCES "gift"("id") ON DELETE CASCADE,
  "code" VARCHAR(50) NOT NULL UNIQUE,
  "sys_status" SMALLINT NOT NULL DEFAULT 0,
  "sys_created" INT NOT NULL,
  "sys_updated" INT NOT NULL
);

-- 创建索引
CREATE INDEX IF NOT EXISTS "idx_coupon_gift_id" ON "coupon" ("gift_id");
CREATE UNIQUE INDEX IF NOT EXISTS "idx_coupon_code" ON "coupon" ("code");
CREATE INDEX IF NOT EXISTS "idx_coupon_sys_status" ON "coupon" ("sys_status");
```

</details>

### 抽奖记录表

```xml
<table name="lottery_record">
    <field name="id" type="int" description="主键，自增"/>
    <field name="gift_id" type="int" description="奖品 ID，关联奖品表"/>
    <field name="gift_name" type="varchar(255)" description="奖品名称"/>
    <field name="gift_type" type="int" description="奖品类型，0-虚拟币，1-虚拟券，2-实物小奖，3-实物大奖"/>
    <field name="uid" type="int" description="用户 ID，关联用户表"/>
    <field name="username" type="varchar(50)" description="用户名"/>
    <field name="prize_code" type="int" description="抽奖编号"/>
    <field name="gift_data" type="varchar(255)" description="奖品数据，JSON 格式，包含券码、实物地址等"/>

    <field name="sys_status" type="small_int" description="系统状态，0-正常，1-已删除"/>
    <field name="sys_created" type="int" description="创建时间，Unix 时间戳"/>
    <field name="sys_updated" type="int" description="更新时间，Unix 时间戳"/>
</table>

```

<details>
  <summary>SQL for postgres - 抽奖记录表</summary>

```sql
CREATE TABLE IF NOT EXISTS "lottery_record" (
  "id" SERIAL PRIMARY KEY,
  "gift_id" INT NOT NULL REFERENCES "gift"("id") ON DELETE RESTRICT,
  "gift_name" VARCHAR(255) NOT NULL,
  "gift_type" INT NOT NULL,
  "uid" INT NOT NULL REFERENCES "user"("id") ON DELETE RESTRICT,
  "username" VARCHAR(50) NOT NULL,
  "prize_code" INT NOT NULL,
  "gift_data" VARCHAR(255) DEFAULT '{}',
  "sys_status" SMALLINT DEFAULT 0,
  "sys_created" INT NOT NULL,
  "sys_updated" INT NOT NULL
);

-- 创建索引
CREATE INDEX IF NOT EXISTS "idx_lottery_record_gift_id" ON "lottery_record" ("gift_id");
CREATE INDEX IF NOT EXISTS "idx_lottery_record_uid" ON "lottery_record" ("uid");
CREATE INDEX IF NOT EXISTS "idx_lottery_record_created" ON "lottery_record" ("sys_created");
CREATE INDEX IF NOT EXISTS "idx_lottery_record_gift_type" ON "lottery_record" ("gift_type");
```

</details>

### 用户黑名单表

```xml
<table name="user_blacklist">
    <field name="id" type="int" description="主键，自增"/>
    <field name="uid" type="int" description="用户 ID，关联用户表"/>
    <field name="end_time" type="int" description="封禁结束时间，Unix 时间戳"/>


    <field name="sys_status" type="small_int" description="系统状态，0-正常，1-已删除"/>
    <field name="sys_created" type="int" description="创建时间，Unix 时间戳"/>
    <field name="sys_updated" type="int" description="更新时间，Unix 时间戳"/>
</table>
```

<details>
  <summary>SQL for postgres - 用户黑名单表</summary>

```sql
CREATE TABLE IF NOT EXISTS "user_blacklist" (
  "id" SERIAL PRIMARY KEY,
  "uid" INT NOT NULL REFERENCES "user"("id") ON DELETE CASCADE, -- 使用 RESTRICT 确保用户删除时不删除黑名单记录, CASCADE 会导致黑名单记录被删除
  "end_time" INT,
  "sys_status" SMALLINT NOT NULL DEFAULT 0,
  "sys_created" INT NOT NULL,
  "sys_updated" INT NOT NULL
);

-- 创建索引
CREATE UNIQUE INDEX IF NOT EXISTS "idx_user_blacklist_uid" ON "user_blacklist" ("uid");
CREATE INDEX IF NOT EXISTS "idx_user_blacklist_end_time" ON "user_blacklist" ("end_time");
CREATE INDEX IF NOT EXISTS "idx_user_blacklist_sys_status" ON "user_blacklist" ("sys_status");
```

</details>

### IP 黑名单表

```xml
<table name="ip_blacklist">
    <field name="id" type="int" description="主键，自增"/>
    <field name="ip" type="varchar(50)" description="IP 地址"/>
    <field name="end_time" type="int" description="封禁结束时间，Unix 时间戳"/>

    <field name="sys_status" type="small_int" description="系统状态，0-正常，1-已删除"/>
    <field name="sys_created" type="int" description="创建时间，Unix 时间戳"/>
    <field name="sys_updated" type="int" description="更新时间，Unix 时间戳"/>
</table>

```

<details>
  <summary>SQL for postgres - IP黑名单表</summary>

```sql
CREATE TABLE IF NOT EXISTS "ip_blacklist" (
  "id" SERIAL PRIMARY KEY,
  "ip" VARCHAR(50) NOT NULL UNIQUE,
  "end_time" INT,
  "sys_status" SMALLINT NOT NULL DEFAULT 0,
  "sys_created" INT NOT NULL,
  "sys_updated" INT NOT NULL
);

-- 创建索引
CREATE UNIQUE INDEX IF NOT EXISTS "idx_ip_blacklist_ip" ON "ip_blacklist" ("ip");
CREATE INDEX IF NOT EXISTS "idx_ip_blacklist_end_time" ON "ip_blacklist" ("end_time");
CREATE INDEX IF NOT EXISTS "idx_ip_blacklist_sys_status" ON "ip_blacklist" ("sys_status");
```

</details>

### 用户每日参与次数表

```xml
<table name="user_participation">
    <field name="id" type="int" description="主键，自增"/>
    <field name="uid" type="int" description="用户 ID，关联用户表"/>
    <field name="date" type="date" description="参与日期"/>
    <field name="count" type="int" description="当天参与次数"/>

    <field name="sys_status" type="small_int" description="系统状态，0-正常，1-已删除"/>
    <field name="sys_created" type="int" description="创建时间，Unix 时间戳"/>
    <field name="sys_updated" type="int" description="更新时间，Unix 时间戳"/>
</table>
```

<details>
  <summary>SQL for postgres - 用户每日参与次数表</summary>

```sql
CREATE TABLE IF NOT EXISTS "user_participation" (
  "id" SERIAL PRIMARY KEY,
  "uid" INT NOT NULL REFERENCES "user"("id") ON DELETE CASCADE,
  "date" DATE NOT NULL,
  "count" INT NOT NULL DEFAULT 0,
  "sys_status" SMALLINT NOT NULL DEFAULT 0,
  "sys_created" INT NOT NULL,
  "sys_updated" INT NOT NULL,
  UNIQUE("date")
);

-- 创建索引
CREATE INDEX IF NOT EXISTS "idx_user_participation_date" ON "user_participation" ("date");
CREATE INDEX IF NOT EXISTS "idx_user_participation_uid" ON "user_participation" ("uid");
```
