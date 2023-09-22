use std::env;

fn str_to_int(number_string: &str) -> Result<i32, &str> {
    number_string.chars().try_rfold(0, |acc, x| {
        if !x.is_numeric() {
            return Err("contained invalid char");
        }
        Ok(acc * 10 + (x as i32 - '0' as i32) as i32)
    })
}

fn waitting_for_godot(brithday: String) -> Result<i32, String> {
    if brithday.len() != 10 {
        return Err("birthay length no right (only 10 with splitcifir)".to_string());
    }

    let mut spliter = "";

    for s in ":-".chars() {
        if brithday.find(s).is_some() {
            spliter = ":";
            break;
        }
    }

    let number_strings: Vec<&str> = brithday.split(spliter).collect();
    if number_strings.len() != 3 {
        return Err("Wrong segment from string".to_string());
    }

    let numbers = number_strings
        .into_iter()
        .map(str_to_int)
        .collect::<Result<Vec<_>, _>>()?;

    let days = numbers[0] * 365 + numbers[1] * 30 + numbers[2];

    Ok(days)
}

fn main() {
    println!("hello world");

    let mut n: String = "shit".to_string();
    if let Some(arg1) = env::args().nth(1) {
        n = arg1.parse().unwrap();
    }

    let days = waitting_for_godot(n.to_string()).unwrap_or_else(|msg| panic!("{}", msg));
    println!("Wating for {}", days);
}
