
import { PrismaClient } from "@prisma/client";

export * as user from "./user"

const prisma = new PrismaClient();


export { prisma };

