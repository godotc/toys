from dotenv import dotenv_values
import openai
import tiktoken


ENV = dotenv_values(".env")
API = ENV.get('API')

openai.api_key = API
