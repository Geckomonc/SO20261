DROP TABLE IF EXISTS result;
DROP TABLE IF EXISTS input;

CREATE TABLE input (
    id SERIAL PRIMARY KEY,
    description TEXT NOT NULL,
    status VARCHAR(20) NOT NULL DEFAULT 'pending',
    CONSTRAINT chk_input_status CHECK (status IN ('pending', 'in_process', 'processed'))
);

CREATE TABLE result (
    id SERIAL PRIMARY KEY,
    input_id INT NOT NULL REFERENCES input(id),
    worker_identifier VARCHAR(50) NOT NULL,
    result TEXT NOT NULL,
    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE UNIQUE INDEX uq_result_input_id ON result(input_id);

INSERT INTO input (description, status) VALUES
('dato 1', 'pending'),
('dato 2', 'pending'),
('dato 3', 'pending'),
('dato 4', 'pending'),
('dato 5', 'pending'),
('dato 6', 'pending'),
('dato 7', 'pending'),
('dato 8', 'pending'),
('dato 9', 'pending'),
('dato 10', 'pending'),
('dato 11', 'pending'),
('dato 12', 'pending'),
('dato 13', 'pending'),
('dato 14', 'pending'),
('dato 15', 'pending'),
('dato 16', 'pending'),
('dato 17', 'pending'),
('dato 18', 'pending'),
('dato 19', 'pending'),
('dato 20', 'pending');