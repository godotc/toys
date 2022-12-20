package logic

import (
	"context"
	"errors"
	"strings"
	"time"

	"book/service/user/api/internal/svc"
	"book/service/user/api/internal/types"

	"github.com/golang-jwt/jwt/v4"
	"github.com/zeromicro/go-zero/core/logx"
)

type LoginLogic struct {
	logx.Logger
	ctx    context.Context
	svcCtx *svc.ServiceContext
}

func NewLoginLogic(ctx context.Context, svcCtx *svc.ServiceContext) *LoginLogic {
	return &LoginLogic{
		Logger: logx.WithContext(ctx),
		ctx:    ctx,
		svcCtx: svcCtx,
	}
}

func (l *LoginLogic) getJwtToken(sceretKey string, iat, seconds, userId int64) (string, error) {
	claims := make(jwt.MapClaims)
	claims["exp"] = iat + seconds
	claims["iat"] = iat
	claims["userId"] = userId
	token := jwt.New(jwt.SigningMethodHS256)
	token.Claims = claims
	return token.SignedString([]byte(sceretKey))

}

func (l *LoginLogic) Login(req *types.LoginReq) (resp *types.LoginReply, err error) {
	// todo: add your logic here and delete this line
	if len(strings.TrimSpace(req.Username)) == 0 || len(strings.TrimSpace(req.Password)) == 0 {
		return nil, errors.New("Argument error!")
	}

	userInfo, err := l.svcCtx.UserModel.FindOneByNumber(l.ctx, req.Username)
	if err != nil {
		return nil, errors.New("Username not exist!")
	}

	if userInfo.Password != req.Password {
		return nil, errors.New("Password not correct!")
	}

	// ts begin
	now := time.Now().Unix()
	accessExpire := l.svcCtx.Config.Auth.AccessExpire
	jwtToken, err := l.getJwtToken(
		l.svcCtx.Config.Auth.AccessSecret,
		now,
		l.svcCtx.Config.Auth.AccessExpire,
		userInfo.Id)
	if err != nil {
		return nil, err
	}

	return &types.LoginReply{
		Id:           userInfo.Id,
		Name:         userInfo.Name,
		Gender:       userInfo.Gender,
		AccessToken:  jwtToken,
		AccessExpire: now + accessExpire,
		RefreshAfter: now + accessExpire/2,
	}, nil
}
