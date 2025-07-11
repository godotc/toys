// Code generated by gorm.io/gen. DO NOT EDIT.
// Code generated by gorm.io/gen. DO NOT EDIT.
// Code generated by gorm.io/gen. DO NOT EDIT.

package query

import (
	"context"
	"fmt"
	"testing"

	"lottery/gen/model"

	"gorm.io/gen"
	"gorm.io/gen/field"
	"gorm.io/gorm/clause"
)

func init() {
	InitializeDB()
	err := _gen_test_db.AutoMigrate(&model.Gift{})
	if err != nil {
		fmt.Printf("Error: AutoMigrate(&model.Gift{}) fail: %s", err)
	}
}

func Test_giftQuery(t *testing.T) {
	gift := newGift(_gen_test_db)
	gift = *gift.As(gift.TableName())
	_do := gift.WithContext(context.Background()).Debug()

	primaryKey := field.NewString(gift.TableName(), clause.PrimaryKey)
	_, err := _do.Unscoped().Where(primaryKey.IsNotNull()).Delete()
	if err != nil {
		t.Error("clean table <gift> fail:", err)
		return
	}

	_, ok := gift.GetFieldByName("")
	if ok {
		t.Error("GetFieldByName(\"\") from gift success")
	}

	err = _do.Create(&model.Gift{})
	if err != nil {
		t.Error("create item in table <gift> fail:", err)
	}

	err = _do.Save(&model.Gift{})
	if err != nil {
		t.Error("create item in table <gift> fail:", err)
	}

	err = _do.CreateInBatches([]*model.Gift{{}, {}}, 10)
	if err != nil {
		t.Error("create item in table <gift> fail:", err)
	}

	_, err = _do.Select(gift.ALL).Take()
	if err != nil {
		t.Error("Take() on table <gift> fail:", err)
	}

	_, err = _do.First()
	if err != nil {
		t.Error("First() on table <gift> fail:", err)
	}

	_, err = _do.Last()
	if err != nil {
		t.Error("First() on table <gift> fail:", err)
	}

	_, err = _do.Where(primaryKey.IsNotNull()).FindInBatch(10, func(tx gen.Dao, batch int) error { return nil })
	if err != nil {
		t.Error("FindInBatch() on table <gift> fail:", err)
	}

	err = _do.Where(primaryKey.IsNotNull()).FindInBatches(&[]*model.Gift{}, 10, func(tx gen.Dao, batch int) error { return nil })
	if err != nil {
		t.Error("FindInBatches() on table <gift> fail:", err)
	}

	_, err = _do.Select(gift.ALL).Where(primaryKey.IsNotNull()).Order(primaryKey.Desc()).Find()
	if err != nil {
		t.Error("Find() on table <gift> fail:", err)
	}

	_, err = _do.Distinct(primaryKey).Take()
	if err != nil {
		t.Error("select Distinct() on table <gift> fail:", err)
	}

	_, err = _do.Select(gift.ALL).Omit(primaryKey).Take()
	if err != nil {
		t.Error("Omit() on table <gift> fail:", err)
	}

	_, err = _do.Group(primaryKey).Find()
	if err != nil {
		t.Error("Group() on table <gift> fail:", err)
	}

	_, err = _do.Scopes(func(dao gen.Dao) gen.Dao { return dao.Where(primaryKey.IsNotNull()) }).Find()
	if err != nil {
		t.Error("Scopes() on table <gift> fail:", err)
	}

	_, _, err = _do.FindByPage(0, 1)
	if err != nil {
		t.Error("FindByPage() on table <gift> fail:", err)
	}

	_, err = _do.ScanByPage(&model.Gift{}, 0, 1)
	if err != nil {
		t.Error("ScanByPage() on table <gift> fail:", err)
	}

	_, err = _do.Attrs(primaryKey).Assign(primaryKey).FirstOrInit()
	if err != nil {
		t.Error("FirstOrInit() on table <gift> fail:", err)
	}

	_, err = _do.Attrs(primaryKey).Assign(primaryKey).FirstOrCreate()
	if err != nil {
		t.Error("FirstOrCreate() on table <gift> fail:", err)
	}

	var _a _another
	var _aPK = field.NewString(_a.TableName(), "id")

	err = _do.Join(&_a, primaryKey.EqCol(_aPK)).Scan(map[string]interface{}{})
	if err != nil {
		t.Error("Join() on table <gift> fail:", err)
	}

	err = _do.LeftJoin(&_a, primaryKey.EqCol(_aPK)).Scan(map[string]interface{}{})
	if err != nil {
		t.Error("LeftJoin() on table <gift> fail:", err)
	}

	_, err = _do.Not().Or().Clauses().Take()
	if err != nil {
		t.Error("Not/Or/Clauses on table <gift> fail:", err)
	}
}
