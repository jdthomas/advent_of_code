use anyhow::Result;
use itertools::Itertools;
use std::collections::HashMap;
use std::collections::HashSet;
use std::collections::VecDeque;

type Point = (i64, i64);

fn manhattan(a: Point, b: Point) -> i64 {
    (a.0 - b.0).abs() + (a.1 - b.1).abs()
}

fn touching(a: Point, b: Point) -> bool {
    (a.0 - b.0).abs() <= 1 && (a.1 - b.1).abs() <= 1
}

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    let mut head = (0, 0);
    let mut tail = (0, 0);
    let mut all_tail_pos = HashSet::new();

    for line in lines {
        // update Head
        let (dir, dist) = line.split_once(" ").unwrap();
        let mut dist = dist.parse::<i64>().unwrap();
        println!("{} {}", dir, dist);
        while dist > 0 {
            match dir {
                "U" => head.0 += 1,
                "D" => head.0 -= 1,
                "L" => head.1 -= 1,
                "R" => head.1 += 1,
                _ => (),
            }

            if !touching(head, tail) {
                let movement = (
                    (head.0 - tail.0).clamp(-1, 1),
                    (head.1 - tail.1).clamp(-1, 1),
                );
                tail = (tail.0 + movement.0, tail.1 + movement.1);
            }
            all_tail_pos.insert(tail);
            dist -= 1;
        }
    }
    println!(
        "Head={:?} tail={:?} tail_visits={:?}",
        head, tail, all_tail_pos
    );
    let grid_max = 6;
    for i in (0..grid_max).rev() {
        println!(
            "{}",
            (0..grid_max)
                .map(|j| {
                    if all_tail_pos.contains(&(i, j)) {
                        if (i, j) == (0, 0) {
                            "s"
                        } else {
                            "#"
                        }
                    } else {
                        "."
                    }
                })
                .join("")
        );
    }
    Ok(all_tail_pos.len() as i64)
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let mut knots: Vec<Point> = vec![(0, 0); 10];
    let mut all_tail_pos = HashSet::new();

    for line in lines {
        let (dir, dist) = line.split_once(" ").unwrap();
        let mut dist = dist.parse::<i64>().unwrap();
        println!("------ > {} {}", dir, dist);

        while dist > 0 {
            // update Head
            match dir {
                "U" => knots[0].0 += 1,
                "D" => knots[0].0 -= 1,
                "L" => knots[0].1 -= 1,
                "R" => knots[0].1 += 1,
                _ => (),
            }

            // Update rest
            let mut head = knots[0];
            knots.iter_mut().skip(1).for_each(|tail| {
                if !touching(head, *tail) {
                    let movement = (
                        (head.0 - tail.0).clamp(-1, 1),
                        (head.1 - tail.1).clamp(-1, 1),
                    );
                    *tail = (tail.0 + movement.0, tail.1 + movement.1);
                }
                head = *tail
            });

            all_tail_pos.insert(knots.last().unwrap().clone());
            dist -= 1;
        }
    }
    println!(
        "Head={:?} tail={:?} tail_visits={:?}",
        knots[0],
        knots.last().unwrap(),
        all_tail_pos
    );
    let grid_min = -12;
    let grid_max = 26;
    for i in (grid_min..grid_max).rev() {
        println!(
            "{}",
            (grid_min..grid_max)
                .map(|j| {
                    if all_tail_pos.contains(&(i, j)) {
                        if (i, j) == (0, 0) {
                            "s"
                        } else {
                            "#"
                        }
                    } else {
                        "."
                    }
                })
                .join("")
        );
    }

    Ok(all_tail_pos.len() as i64)
}
