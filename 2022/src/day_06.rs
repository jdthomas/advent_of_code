use anyhow::Result;

use std::collections::HashSet;

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    let result = lines
        .iter()
        .map(|line| {
            let l = line.chars().collect::<Vec<char>>();
            let sti = l.windows(4).position(|w| {
                let s: HashSet<char> = w.iter().cloned().collect();
                s.len() == 4
            });
            println!("{:?}", sti.unwrap() + 4);
            sti.unwrap() + 4
        })
        .max()
        .unwrap();
    Ok(result as i64)
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let result = lines
        .iter()
        .map(|line| {
            let l = line.chars().collect::<Vec<char>>();
            let sti = l.windows(14).position(|w| {
                let s: HashSet<char> = w.iter().cloned().collect();
                s.len() == 14
            });
            println!("{:?}", sti.unwrap() + 14);
            sti.unwrap() + 14
        })
        .max()
        .unwrap();
    Ok(result as i64)
}
