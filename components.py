def desk_components(fingerCount) -> str:
    if fingerCount == 0:
        return "None"
    elif fingerCount == 1:
        return "Scissor"
    elif fingerCount == 2:
        return "Scalpel"
    elif fingerCount == 3:
        return "Blade"
    else:
        return "Invalid"