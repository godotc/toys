from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

model_name = "gpt2-medium"
model = AutoModelForCausalLM.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name)


def generate_response(prompt, model, tokenizer):
    input_ids = tokenizer.encode(prompt, return_tensors="pt")
    attention_mask = torch.ones_like(input_ids)
    pad_token_id = tokenizer.eos_token_id
    output = model.generate(
        input_ids,
        attention_mask=attention_mask,
        pad_token_id=pad_token_id,
        max_length=1000,
        do_sample=True
    )
    response = tokenizer.decode(output[0], skip_special_tokens=True)
    return response


while True:
    prompt = input("Here to input: \n")
    response = generate_response(prompt, model, tokenizer)
    print("Bot: "+response)
