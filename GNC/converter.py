
from sympy import Symbol, sympify, Expr
import re

def extract_symbols(string: str) -> set[str]:
    pattern = r'[a-zA-Z0-9]+(?:\([a-zA-Z0-9]*\))?'
    symbols_ = re.findall(pattern, string)
    return set(symbols_)

def extract_symbols_from_system(system: list[str]) -> set[str]:
    total_symbols = set()
    for string in system:
        total_symbols.update(extract_symbols(string))
    return total_symbols

def extract_coefficient(eqn: Expr, sym: Symbol) -> Expr:
    return sum(term.coeff(sym) for term in eqn.as_ordered_terms() if sym in term.free_symbols)

def convert_to_matrix(system: list[Expr], vector_terms: list[Symbol]):
    """
    :param system: the system of equations must be in the order of the vector terms ([n] dimensional)
    :param vector_terms: a list of symbols in the order of the input vector ([n] dimensional)
    :return: a matrix corresponding to the transfer matrix from the input vector to the output equation ([nxn] dimensional)
    """
    matrix = []
    for eqn in system:
        row = []
        for vec_term in vector_terms:
            row.append(extract_coefficient(eqn, vec_term))
        matrix.append(row)
    return matrix

def extract_constants(system: list[Expr], disallowed_symbols: list[Symbol]) -> list[Expr]:
    vector = []
    for eqn in system:
        constant: Expr = sum(term for term in eqn.as_ordered_terms() if all([sym not in term.free_symbols for sym in disallowed_symbols]))
        vector.append(constant)
    return vector

def convert_matrix_to_string(matrix: list[list[Expr]]):
    return [[str(term).replace(' ', '') for term in row] for row in matrix]

def print_matrix(matrix: list[list[str]]):
    stdout = "[\n"
    for row in matrix:
        stdout += '\t' + ' '.join(row) + ';\n'
    return stdout.replace('**', '^') + '];\n'

def print_vector(vector: list[str]):
    return print_matrix([[term] for term in vector])

def print_system(system: list[str], x_variables: list[str], u_variables: list[str]):
    symbols = [Symbol(sym) for sym in sorted(list(extract_symbols_from_system(system)))]

    x_symbols = {k: [sym for sym in symbols if sym.name == k][0] for k in x_variables}
    u_symbols = {k: [sym for sym in symbols if sym.name == k][0] for k in u_variables}
    c_symbols = {sym.name: sym for sym in symbols if sym.name not in x_symbols and sym.name not in u_symbols}

    system = [sympify(eqn, locals=(x_symbols | u_symbols | c_symbols)) for eqn in system]
    system = [eqn.expand() for eqn in system]

    A_mat = convert_to_matrix(system, list(x_symbols.values()))
    B_mat = convert_to_matrix(system, list(u_symbols.values()))
    d_vec = extract_constants(system, list((x_symbols | u_symbols).values()))

    return (print_matrix(convert_matrix_to_string(A_mat)),
            print_matrix(convert_matrix_to_string(B_mat)),
            print_vector([str(term) for term in d_vec]))

def write_to_script(file: str, start_string: str, end_string: str, paste: str):
    file_content = open(file).read()
    head = file_content[:file_content.find(start_string) + len(start_string)]
    tail = file_content[file_content.find(end_string):]
    with open(file, 'w+') as f:
        f.write(head)
        f.write('\n')
        f.write(paste)
        f.write(tail)

def indent(string: str):
    string = string.replace('\t', '    ')
    string = string.replace('\n', '\n    ')
    return '    ' + string

def update_matlab_script(system: list[str], x_variables: list[str], u_variables: list[str]):
    A_mat, B_mat, d_vec = print_system(system, x_variables, u_variables)
    write_to_script(
        'lqrMatrices.m',
        '% BEGIN "A"; Do not modify this. The python script looks for this line.',
        '% END "A"; Do not modify this. The python script looks for this line.',
        indent('A = ' + A_mat)
    )
    write_to_script(
        'lqrMatrices.m',
        '% BEGIN "B"; Do not modify this. The python script looks for this line.',
        '% END "B"; Do not modify this. The python script looks for this line.',
        indent('B = ' + B_mat)
    )
    write_to_script(
        'lqrMatrices.m',
        '% BEGIN "d"; Do not modify this. The python script looks for this line.',
        '% END "d"; Do not modify this. The python script looks for this line.',
        indent('d = ' + d_vec)
    )


if __name__ == '__main__':

    u_variables = ['T', 'tauR', 'xi', 'zeta']
    x_variables = ['x', 'y', 'z', 'u', 'v', 'w', 'phi', 'theta', 'psi', 'p', 'q', 'r']
    # Copied and pasted from the output of "eomAlgebra2.m" file.
    # Keep in mind though that the equations need to be in the order
    # of the x_variables.
    system = [
        "u - q0*z0 - r0*y0 - z0*(q - q0) - y0*(r - r0) - r0*(y - y0) - q0*(z - z0)",
        "v + p0*z0 - r0*x0 + z0*(p - p0) - x0*(r - r0) - r0*(x - x0) + p0*(z - z0)",
        "w - p0*y0 + q0*x0 - y0*(p - p0) + x0*(q - q0) + q0*(x - x0) - p0*(y - y0)",
        "(T*cos(xi0)*cos(zeta0) - M*q*w0 - M*q0*w + M*q0*w0 + M*r*v0 + M*r0*v - M*r0*v0 - M*g*cos(psi0)*cos(theta0) - T0*xi*cos(zeta0)*sin(xi0) + T0*xi0*cos(zeta0)*sin(xi0) + T0*zeta0*cos(xi0)*sin(zeta0) - T0*zeta*cos(xi0)*sin(zeta0) - M*g*psi0*cos(theta0)*sin(psi0) + M*g*psi*cos(theta0)*sin(psi0) + M*g*theta*cos(psi0)*sin(theta0) - M*g*theta0*cos(psi0)*sin(theta0))/M",
        "(T*sin(zeta0) - T0*zeta0*cos(zeta0) + T0*zeta*cos(zeta0) + M*p*w0 + M*p0*w - M*p0*w0 - M*r*u0 - M*r0*u + M*r0*u0 + M*g*cos(phi0)*sin(psi0) - M*g*psi0*cos(phi0)*cos(psi0) + M*g*psi*cos(phi0)*cos(psi0) - M*g*phi*sin(phi0)*sin(psi0) + M*g*phi0*sin(phi0)*sin(psi0) - M*g*cos(psi0)*sin(phi0)*sin(theta0) - M*g*phi*cos(phi0)*cos(psi0)*sin(theta0) + M*g*phi0*cos(phi0)*cos(psi0)*sin(theta0) - M*g*theta*cos(psi0)*cos(theta0)*sin(phi0) + M*g*theta0*cos(psi0)*cos(theta0)*sin(phi0) - M*g*psi0*sin(phi0)*sin(psi0)*sin(theta0) + M*g*psi*sin(phi0)*sin(psi0)*sin(theta0))/M",
        "-(T*cos(zeta0)*sin(xi0) + M*p*v0 + M*p0*v - M*p0*v0 - M*q*u0 - M*q0*u + M*q0*u0 + T0*zeta0*sin(xi0)*sin(zeta0) - T0*zeta*sin(xi0)*sin(zeta0) + M*g*sin(phi0)*sin(psi0) + T0*xi*cos(xi0)*cos(zeta0) - T0*xi0*cos(xi0)*cos(zeta0) + M*g*phi*cos(phi0)*sin(psi0) - M*g*phi0*cos(phi0)*sin(psi0) - M*g*psi0*cos(psi0)*sin(phi0) + M*g*psi*cos(psi0)*sin(phi0) + M*g*cos(phi0)*cos(psi0)*sin(theta0) + M*g*theta*cos(phi0)*cos(psi0)*cos(theta0) - M*g*theta0*cos(phi0)*cos(psi0)*cos(theta0) - M*g*phi*cos(psi0)*sin(phi0)*sin(theta0) + M*g*phi0*cos(psi0)*sin(phi0)*sin(theta0) + M*g*psi0*cos(phi0)*sin(psi0)*sin(theta0) - M*g*psi*cos(phi0)*sin(psi0)*sin(theta0))/M",
        "p + (r0*cos(phi0)*(tan(theta0)^2 + 1) + q0*sin(phi0)*(tan(theta0)^2 + 1))*(theta - theta0) + (q0*cos(phi0)*tan(theta0) - r0*sin(phi0)*tan(theta0))*(phi - phi0) + r0*cos(phi0)*tan(theta0) + q0*sin(phi0)*tan(theta0) + cos(phi0)*tan(theta0)*(r - r0) + sin(phi0)*tan(theta0)*(q - q0)",
        "q0*cos(phi0) - r0*sin(phi0) - (phi - phi0)*(r0*cos(phi0) + q0*sin(phi0)) + cos(phi0)*(q - q0) - sin(phi0)*(r - r0)",
        "((q0*cos(phi0))/cos(theta0) - (r0*sin(phi0))/cos(theta0))*(phi - phi0) + (theta - theta0)*((r0*cos(phi0)*sin(theta0))/cos(theta0)^2 + (q0*sin(phi0)*sin(theta0))/cos(theta0)^2) + (cos(phi0)*(r - r0))/cos(theta0) + (sin(phi0)*(q - q0))/cos(theta0) + (r0*cos(phi0))/cos(theta0) + (q0*sin(phi0))/cos(theta0)",
        "(tauR + Iyy*q*r0 + Iyy*q0*r - Iyy*q0*r0 - Izz*q*r0 - Izz*q0*r + Izz*q0*r0)/Ixx",
        "-(Ixx*p*r0 + Ixx*p0*r - Ixx*p0*r0 - Izz*p*r0 - Izz*p0*r + Izz*p0*r0 + T*rho2*cos(zeta0)*sin(xi0) + T0*rho2*xi*cos(xi0)*cos(zeta0) - T0*rho2*xi0*cos(xi0)*cos(zeta0) + T0*rho2*zeta0*sin(xi0)*sin(zeta0) - T0*rho2*zeta*sin(xi0)*sin(zeta0))/Iyy",
        "-(T*rho2*sin(zeta0) - Ixx*p*q0 - Ixx*p0*q + Ixx*p0*q0 + Iyy*p*q0 + Iyy*p0*q - Iyy*p0*q0 - T0*rho2*zeta0*cos(zeta0) + T0*rho2*zeta*cos(zeta0))/Izz"
    ]

    update_matlab_script(system, x_variables, u_variables)

