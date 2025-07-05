package main

import (
	"fmt"
	"log"
	"os"

	"github.com/joho/godotenv"
	"gorm.io/driver/postgres"
	"gorm.io/gen"
	"gorm.io/gorm"
)

func main() {

	err := godotenv.Load()
	if err != nil {
		log.Fatal("Error loading .env file")
	}

	db, err := gorm.Open(
		postgres.New(postgres.Config{
			DSN: fmt.Sprintf("user=%s password=%s host=%s port=%s dbname=%s sslmode=disable TimeZone=Asia/Shanghai",
				os.Getenv("PG_USER"),
				os.Getenv("PG_PASSWORD"),
				os.Getenv("PG_HOST"),
				os.Getenv("PG_PORT"),
				os.Getenv("PG_DB_NAME"),
			),
		}),

		&gorm.Config{},
	)
	if err != nil {
		log.Fatal("Failed to connect to database:", err)
	}

	g := gen.NewGenerator(
		gen.Config{
			OutPath:      "./gen/query",
			ModelPkgPath: "model", // relative  to the ../output  -> ./gen/model
			Mode:         gen.WithDefaultQuery | gen.WithQueryInterface | gen.WithoutContext,
			// FieldNullable: false,
			WithUnitTest: true,
		})
	g.UseDB(db)
	g.ApplyBasic(
		g.GenerateAllTable()...,
	)
	g.Execute()
}
