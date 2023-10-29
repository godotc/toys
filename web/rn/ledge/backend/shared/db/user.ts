import { PrismaClient } from "@prisma/client";

import { prisma } from "./index";

type NewUserProps = {
    name: string;
    email: string;
    password: string;
};

// Method to create a new user
export const createUser = async (params: NewUserProps) => {
    try {
        const existingUser = await prisma.user.findFirst({
            where: {
                email: params.email,
            },
        });

        if (existingUser) {
            return Promise.reject("User with this email already exists");
        }

        const user = await prisma.user.create({
            data: {
                name: params.name,
                email: params.email,
                password: params.password,
            },
        });

        return user;
    } catch (error: any) {
        return Promise.reject(error.message);
    }
};

export const updatePassword = async (email: string, old_password: string, new_password: string) => {
    const user = await prisma.user.update(
        {
            where: {
                email,
                password: old_password,
            },
            data: {
                password: new_password
            }
        }
    )
    return user
}

// Method to get a user by their email
export const getUserByEmail = async (email: string) => {
    const user = await prisma.user.findUnique({
        where: {
            email,
        },
    });
    return user;
};

// Method to delete a user by their email
export const deleteUserByEmail = async (email: string) => {
    await prisma.user.delete({
        where: {
            email,
        },
    });
};


