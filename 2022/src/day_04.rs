use anyhow::Result;



fn covers(a_min:&str, a_max:&str, b_min:&str, b_max:&str) -> bool {
    let a_min: i64 = a_min.parse::<i64>().unwrap();
    let a_max: i64 = a_max.parse::<i64>().unwrap();
    let b_min: i64 = b_min.parse::<i64>().unwrap();
    let b_max: i64 = b_max.parse::<i64>().unwrap();

    (a_min <= b_min && a_max >= b_max) || (b_min <= a_min && b_max >= a_max)
}

fn overlaps(a_min:&str, a_max:&str, b_min:&str, b_max:&str) -> bool {
    let a_min: i64 = a_min.parse::<i64>().unwrap();
    let a_max: i64 = a_max.parse::<i64>().unwrap();
    let b_min: i64 = b_min.parse::<i64>().unwrap();
    let b_max: i64 = b_max.parse::<i64>().unwrap();

    a_min <= b_max && b_min <= a_max
}

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    let result = lines.iter().filter(|line| {
        let (a,b) = line.split_once(',').unwrap();
        let (a_min,a_max) = a.split_once('-').unwrap();
        let (b_min, b_max) = b.split_once('-').unwrap();
        covers(a_min, a_max, b_min, b_max)
    }).count() as i64;
    Ok(result)
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let result = lines.iter().filter(|line| {
        let (a,b) = line.split_once(',').unwrap();
        let (a_min,a_max) = a.split_once('-').unwrap();
        let (b_min, b_max) = b.split_once('-').unwrap();
        overlaps(a_min, a_max, b_min, b_max)
    }).count() as i64;
    Ok(result)}
