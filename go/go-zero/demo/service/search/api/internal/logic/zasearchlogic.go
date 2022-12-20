package logic

import (
	"context"

	"book/service/search/api/internal/svc"
	"book/service/search/api/internal/types"

	"github.com/zeromicro/go-zero/core/logx"
)

type ZAsearchLogic struct {
	logx.Logger
	ctx    context.Context
	svcCtx *svc.ServiceContext
}

func NewZAsearchLogic(ctx context.Context, svcCtx *svc.ServiceContext) *ZAsearchLogic {
	return &ZAsearchLogic{
		Logger: logx.WithContext(ctx),
		ctx:    ctx,
		svcCtx: svcCtx,
	}
}

func (l *ZAsearchLogic) ZAsearch(req *types.SearchReq) (resp *types.SearchReply, err error) {
	// todo: add your logic here and delete this line

	return
}
