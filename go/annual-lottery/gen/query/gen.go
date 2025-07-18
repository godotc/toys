// Code generated by gorm.io/gen. DO NOT EDIT.
// Code generated by gorm.io/gen. DO NOT EDIT.
// Code generated by gorm.io/gen. DO NOT EDIT.

package query

import (
	"context"
	"database/sql"

	"gorm.io/gorm"

	"gorm.io/gen"

	"gorm.io/plugin/dbresolver"
)

var (
	Q                 = new(Query)
	Coupon            *coupon
	Gift              *gift
	IPBlacklist       *iPBlacklist
	LotteryRecord     *lotteryRecord
	User              *user
	UserBlacklist     *userBlacklist
	UserParticipation *userParticipation
)

func SetDefault(db *gorm.DB, opts ...gen.DOOption) {
	*Q = *Use(db, opts...)
	Coupon = &Q.Coupon
	Gift = &Q.Gift
	IPBlacklist = &Q.IPBlacklist
	LotteryRecord = &Q.LotteryRecord
	User = &Q.User
	UserBlacklist = &Q.UserBlacklist
	UserParticipation = &Q.UserParticipation
}

func Use(db *gorm.DB, opts ...gen.DOOption) *Query {
	return &Query{
		db:                db,
		Coupon:            newCoupon(db, opts...),
		Gift:              newGift(db, opts...),
		IPBlacklist:       newIPBlacklist(db, opts...),
		LotteryRecord:     newLotteryRecord(db, opts...),
		User:              newUser(db, opts...),
		UserBlacklist:     newUserBlacklist(db, opts...),
		UserParticipation: newUserParticipation(db, opts...),
	}
}

type Query struct {
	db *gorm.DB

	Coupon            coupon
	Gift              gift
	IPBlacklist       iPBlacklist
	LotteryRecord     lotteryRecord
	User              user
	UserBlacklist     userBlacklist
	UserParticipation userParticipation
}

func (q *Query) Available() bool { return q.db != nil }

func (q *Query) clone(db *gorm.DB) *Query {
	return &Query{
		db:                db,
		Coupon:            q.Coupon.clone(db),
		Gift:              q.Gift.clone(db),
		IPBlacklist:       q.IPBlacklist.clone(db),
		LotteryRecord:     q.LotteryRecord.clone(db),
		User:              q.User.clone(db),
		UserBlacklist:     q.UserBlacklist.clone(db),
		UserParticipation: q.UserParticipation.clone(db),
	}
}

func (q *Query) ReadDB() *Query {
	return q.ReplaceDB(q.db.Clauses(dbresolver.Read))
}

func (q *Query) WriteDB() *Query {
	return q.ReplaceDB(q.db.Clauses(dbresolver.Write))
}

func (q *Query) ReplaceDB(db *gorm.DB) *Query {
	return &Query{
		db:                db,
		Coupon:            q.Coupon.replaceDB(db),
		Gift:              q.Gift.replaceDB(db),
		IPBlacklist:       q.IPBlacklist.replaceDB(db),
		LotteryRecord:     q.LotteryRecord.replaceDB(db),
		User:              q.User.replaceDB(db),
		UserBlacklist:     q.UserBlacklist.replaceDB(db),
		UserParticipation: q.UserParticipation.replaceDB(db),
	}
}

type queryCtx struct {
	Coupon            ICouponDo
	Gift              IGiftDo
	IPBlacklist       IIPBlacklistDo
	LotteryRecord     ILotteryRecordDo
	User              IUserDo
	UserBlacklist     IUserBlacklistDo
	UserParticipation IUserParticipationDo
}

func (q *Query) WithContext(ctx context.Context) *queryCtx {
	return &queryCtx{
		Coupon:            q.Coupon.WithContext(ctx),
		Gift:              q.Gift.WithContext(ctx),
		IPBlacklist:       q.IPBlacklist.WithContext(ctx),
		LotteryRecord:     q.LotteryRecord.WithContext(ctx),
		User:              q.User.WithContext(ctx),
		UserBlacklist:     q.UserBlacklist.WithContext(ctx),
		UserParticipation: q.UserParticipation.WithContext(ctx),
	}
}

func (q *Query) Transaction(fc func(tx *Query) error, opts ...*sql.TxOptions) error {
	return q.db.Transaction(func(tx *gorm.DB) error { return fc(q.clone(tx)) }, opts...)
}

func (q *Query) Begin(opts ...*sql.TxOptions) *QueryTx {
	tx := q.db.Begin(opts...)
	return &QueryTx{Query: q.clone(tx), Error: tx.Error}
}

type QueryTx struct {
	*Query
	Error error
}

func (q *QueryTx) Commit() error {
	return q.db.Commit().Error
}

func (q *QueryTx) Rollback() error {
	return q.db.Rollback().Error
}

func (q *QueryTx) SavePoint(name string) error {
	return q.db.SavePoint(name).Error
}

func (q *QueryTx) RollbackTo(name string) error {
	return q.db.RollbackTo(name).Error
}
